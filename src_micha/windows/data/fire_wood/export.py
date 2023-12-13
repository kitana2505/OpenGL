'''
This script exports mesh as a C/C++ or JSON source code.
Check the supplementary README.txt file for more info and detailed format description.
'''

bl_info = {
	'name': 'PGR Mesh C++/JSON Source Exporter',
	'author': 'Tomas Barak (initial version), Jaroslav Sloup (update to 2.8)',
	'blender': (2,80,0),
	'version': (0,2,0),
	'location': 'File > Import-Export',
	'description': 'PGR Mesh C++/JSON Source Exporter',
	'category': 'Import-Export'
}

import bpy

def floatCmp(a, b, precision = 0.00001):
	return abs(a - b) <= abs(a + b) * precision

def safeName(name):
	import re
	return re.sub('[^a-zA-Z0-9]+', '_', name).lower()

class PgrCppVertex:
	def __init__(self, x, y, z, nx, ny, nz, u = 0.5, v = 0.5):
		self.x = float(x)
		self.y = float(y)
		self.z = float(z)
		self.nx = float(round(nx, 6))
		self.ny = float(round(ny, 6))
		self.nz = float(round(nz, 6))
		self.u = float(round(u, 6))
		self.v = float(round(v, 6))

	def __eq__(self, v):
		return floatCmp(self.x, v.x) and floatCmp(self.y, v.y) and floatCmp(self.z, v.z)\
			and floatCmp(self.nx, v.nx) and floatCmp(self.ny, v.ny) and floatCmp(self.nz, v.nz)\
			and floatCmp(self.u, v.u) and floatCmp(self.v, v.v)

	def __hash__(self):
		return hash(self.x) ^ hash(self.y) ^ hash(self.z)\
			^ hash(self.nx) ^ hash(self.ny) ^ hash(self.nz)\
			^ hash(self.u) ^ hash(self.v)

	def __str__(self):
		return '{},{},{},{},{},{},{},{}'.format(self.x, self.y, self.z, self.nx, self.ny, self.nz, self.u, self.v)

class ExporterError(RuntimeError):
	pass

class DefaultFaceUV:
	def __init__(self):
		self.uv = [[0.5,0.5],[0.5,0.5],[0.5,0.5],[0.5,0.5]]

class MeshFilter:
	def __init__(self, context):
		self.vtxDict = dict()
		self.vtxList = list()
		self.faceList = list()
		self.obj = context.active_object
		self.safename = safeName(self.obj.name)
		try:
			# old stuff Blender 2.7.x
			#self.mesh = self.obj.to_mesh(context.scene, True, 'PREVIEW')
			#self.mesh = self.obj.to_mesh(True, context.evaluated_depsgraph_get())
			depsgraph = bpy.context.evaluated_depsgraph_get()
			object_eval = self.obj.evaluated_get(depsgraph)
			self.mesh = bpy.data.meshes.new_from_object(object_eval)
		except RuntimeError:
			raise ExporterError('selected object is not a mesh')

	def processTriangle(self, triIn, smooth, faceNormal, faceUVs = [[0.5,0.5],[0.5,0.5],[0.5,0.5]]):
		for i in range(0, 3):
			bvtx = self.mesh.vertices[triIn[i]]
			vtx = None
			if smooth:
				vtx = PgrCppVertex(bvtx.co[0], bvtx.co[1], bvtx.co[2],\
						bvtx.normal[0], bvtx.normal[1], bvtx.normal[2], faceUVs[i][0], faceUVs[i][1])
			else:
				vtx = PgrCppVertex(bvtx.co[0], bvtx.co[1], bvtx.co[2],\
						faceNormal[0], faceNormal[1], faceNormal[2], faceUVs[i][0], faceUVs[i][1])
			if vtx in self.vtxDict:
				#print('vertex {} already in'.format(vtx))
				triIn[i] = self.vtxDict[vtx]
			else:
				self.vtxDict[vtx] = triIn[i] = len(self.vtxDict)
				self.vtxList.append(vtx)
		return triIn

	def process(self):
		#old stuff Blender 2.7.x
		#uvs = self.mesh.tessface_uv_textures[0].data if len(self.mesh.tessface_uv_textures) else [DefaultFaceUV()] * len(self.mesh.tessfaces)
		#for f, fuv in zip(self.mesh.tessfaces, uvs):
		#	self.faceList.append(self.processTriangle([f.vertices[0], f.vertices[1], f.vertices[2]], f.use_smooth, f.normal, fuv.uv))
		#	if len(f.vertices) == 4:
		#		self.faceList.append(self.processTriangle([f.vertices[0], f.vertices[2], f.vertices[3]], f.use_smooth, f.normal, [fuv.uv[0], fuv.uv[2], fuv.uv[3]]))
		self.mesh.calc_loop_triangles()
		uv_layer = self.mesh.uv_layers[0].data if len(self.mesh.uv_layers) else [DefaultFaceUV()] * len(self.mesh.loop_triangles)
		for tri in self.mesh.loop_triangles:
			self.faceList.append(self.processTriangle(\
					[self.mesh.loops[tri.loops[0]].vertex_index, self.mesh.loops[tri.loops[1]].vertex_index, self.mesh.loops[tri.loops[2]].vertex_index],\
					tri.use_smooth, tri.normal,\
					[uv_layer[tri.loops[0]].uv, uv_layer[tri.loops[1]].uv, uv_layer[tri.loops[2]].uv]))

class AbstractWriter:
	def __init__(self, meshfilter, filename):
		self.meshfilter = meshfilter
		self.filename = filename
	def write(self):
		raise NotImplementedError('subclass must implement this method')
	def formatVertex(self, v):
		raise NotImplementedError('subclass must implement this method')

class CppWriter(AbstractWriter):
	def __init__(self, meshfilter, filename, hcpppair):
		super().__init__(meshfilter, filename)
		self.hcpppair = hcpppair
	def formatVertex(self, v):
		f = '{:.6}f,{:.6}f,{:.6}f,{:.6}f,{:.6}f,{:.6}f,{:.6}f,{:.6}f'
		return f.format(v.x, v.y, v.z, v.nx, v.ny, v.nz, v.u, v.v)
	def write(self):
		import os
		noext = os.path.splitext(self.filename)[0]
		dataname = noext + '.cpp' if self.hcpppair else self.filename
		headername = noext + '.h'
		print('----- Writing mesh to ' + dataname + ' -----')
		with open(dataname, 'w') as out:
			if self.hcpppair:
				out.write('#include "{}"\n'.format(os.path.basename(headername)))
			out.write('const int {}NAttribsPerVertex = 8;\n'.format(self.meshfilter.safename))
			out.write('const int {}NVertices = {};\n'.format(self.meshfilter.safename, len(self.meshfilter.vtxList)))
			out.write('const int {}NTriangles = {};\n'.format(self.meshfilter.safename, len(self.meshfilter.faceList)))
			out.write('const float {}Vertices[] = {{\n'.format(self.meshfilter.safename))
			for v in self.meshfilter.vtxList:
				out.write('  {},\n'.format(self.formatVertex(v)))
			out.write('}}; // end {}Vertices\n\n'.format(self.meshfilter.safename))
			out.write('const unsigned {}Triangles[] = {{\n'.format(self.meshfilter.safename))
			for f in self.meshfilter.faceList:
				out.write('  {}, {}, {},\n'.format(f[0], f[1], f[2]))
			out.write('}}; // end {}Triangles\n\n'.format(self.meshfilter.safename))
		if self.hcpppair:
			print('----- Writing header to ' + headername + ' -----')
			with open(headername, 'w') as out:
				out.write('#pragma once\n')
				out.write('extern const int {}NAttribsPerVertex;\n'.format(self.meshfilter.safename))
				out.write('extern const int {}NVertices;\n'.format(self.meshfilter.safename))
				out.write('extern const int {}NTriangles;\n'.format(self.meshfilter.safename))
				out.write('extern const float {}Vertices[];\n'.format(self.meshfilter.safename))
				out.write('extern const unsigned {}Triangles[];\n'.format(self.meshfilter.safename))
		print('----- Done -----')

class JsonWriter(AbstractWriter):
	def formatVertex(self, v):
		f = '{:.6},{:.6},{:.6},{:.6},{:.6},{:.6},{:.6},{:.6}'
		return f.format(v.x, v.y, v.z, v.nx, v.ny, v.nz, v.u, v.v)
	def write(self):
		print('----- Writing mesh to ' + self.filename + ' -----')
		with open(self.filename, 'w') as out:
			out.write('{{"name":"{}","nVertices":{},"nTriangles":{},"nAttribsPerVertex":8,'\
				.format(self.meshfilter.safename, len(self.meshfilter.vtxList), len(self.meshfilter.faceList)))
			out.write('"verticesInterleaved":[')
			for v in self.meshfilter.vtxList:
				out.write('{},'.format(self.formatVertex(v)))
			out.write('],"triangles":[')
			for f in self.meshfilter.faceList:
				out.write('{},{},{},'.format(f[0], f[1], f[2]))
			out.write(']}')
		print('----- Done -----')

class ExportMyFormat(bpy.types.Operator):
	bl_idname = 'export_mesh.pgr'
	bl_label = 'Export PGR source code'
	bl_options = {'PRESET'}

	filepath = bpy.props.StringProperty(subtype='FILE_PATH')
	filetype = bpy.props.EnumProperty(items=[('CPP', 'C/C++', 'Export as C/C++ arrays', 1),('JSON', 'JSON', 'Export as JSON object (for WebGL)', 2)], name='Language')
	hcpppair = bpy.props.BoolProperty(name='Generate .h .cpp pair', default=True, description='Do not write data directly to .h. No effect on json exporter.')
	filetype_ext = {'CPP': '.h', 'JSON': '.js'}

	def invoke(self, context, event):
		#print('-- invoke --')
		import os
		if not self.filepath:
			blend_filepath = context.blend_data.filepath
			if not blend_filepath:
				try:
					blend_filepath = safeName(context.active_object.name)
				except AttributeError:
					blend_filepath = "untitled"
			else:
				blend_filepath = os.path.splitext(blend_filepath)[0]
			self.filepath = blend_filepath + self.filetype_ext['CPP']
		context.window_manager.fileselect_add(self)
		return {'RUNNING_MODAL'}

	def check(self, context):
		#print('-- check --')
		import os
		changed = False
		check_ext = True

		if check_ext:
			filepath = self.filepath
			if os.path.basename(filepath):
				type_ext = self.filetype_ext[self.filetype]
				filepath = bpy.path.ensure_ext(filepath, type_ext)
			if filepath != self.filepath:
				self.filepath = filepath
				changed = True
		return changed

	def execute(self, context):
		#print('-- execute --')
		try:
			exporter = MeshFilter(context)
			exporter.process()
			writer = CppWriter(exporter, self.filepath, self.hcpppair) if self.filetype == 'CPP' else JsonWriter(exporter, self.filepath)
			writer.write()
			return {'FINISHED'}
		except ExporterError as e:
			self.report({'ERROR'}, str(e))
			return {'CANCELLED'}


def menu_func(self, context):
	self.layout.operator(ExportMyFormat.bl_idname, text='PGR Model C++/JSON Source (.h/.cpp/.js)')

classes = (
	ExportMyFormat,
)

def register():
	#old stuff Blender 2.7.x
	#bpy.utils.register_module(__name__)
	from bpy.utils import register_class
	for cls in classes:
		register_class(cls)
	bpy.types.TOPBAR_MT_file_export.append(menu_func)

def unregister():
	#old stuff Blender 2.7.x
	#bpy.utils.unregister_module(__name__)
	from bpy.utils import unregister_class
	for cls in reversed(classes):
		unregister_class(cls)
	bpy.types.TOPBAR_MT_file_export.remove(menu_func)

if __name__ == '__main__':
	register()