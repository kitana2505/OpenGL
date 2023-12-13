#include "fireplace_big.h"
const int fire_bigNAttribsPerVertex = 8;
const int fire_bigNVertices = 216;
const int fire_bigNTriangles = 120;
const float fire_bigVertices[] = {
  0.0f,1.0f,-1.0f,0.5f,0.866025f,0.0f,1.0f,0.5f,
  0.0f,1.0f,1.0f,0.5f,0.866025f,0.0f,1.0f,1.0f,
  0.866025f,0.5f,1.0f,0.5f,0.866025f,0.0f,0.833333f,1.0f,
  0.866025f,0.5f,-1.0f,0.5f,0.866025f,-0.0f,0.833333f,0.5f,
  0.866025f,0.5f,-1.0f,1.0f,-0.0f,0.0f,0.833333f,0.5f,
  0.866025f,0.5f,1.0f,1.0f,-0.0f,0.0f,0.833333f,1.0f,
  0.866025f,-0.5f,1.0f,1.0f,-0.0f,0.0f,0.666667f,1.0f,
  0.866025f,-0.5f,-1.0f,1.0f,-0.0f,0.0f,0.666667f,0.5f,
  0.866025f,-0.5f,-1.0f,0.5f,-0.866025f,0.0f,0.666667f,0.5f,
  0.866025f,-0.5f,1.0f,0.5f,-0.866025f,0.0f,0.666667f,1.0f,
  0.0f,-1.0f,1.0f,0.5f,-0.866025f,0.0f,0.5f,1.0f,
  0.0f,-1.0f,-1.0f,0.5f,-0.866025f,0.0f,0.5f,0.5f,
  0.0f,-1.0f,-1.0f,-0.5f,-0.866025f,-0.0f,0.5f,0.5f,
  0.0f,-1.0f,1.0f,-0.5f,-0.866025f,-0.0f,0.5f,1.0f,
  -0.866025f,-0.5f,1.0f,-0.5f,-0.866025f,-0.0f,0.333333f,1.0f,
  -0.866025f,-0.5f,-1.0f,-0.5f,-0.866025f,0.0f,0.333333f,0.5f,
  0.866025f,-0.5f,1.0f,0.0f,0.0f,1.0f,0.457846f,0.13f,
  0.866025f,0.5f,1.0f,0.0f,0.0f,1.0f,0.457846f,0.37f,
  0.0f,1.0f,1.0f,0.0f,0.0f,1.0f,0.25f,0.49f,
  -0.866025f,0.5f,1.0f,0.0f,0.0f,1.0f,0.042154f,0.37f,
  -0.866025f,-0.5f,1.0f,0.0f,0.0f,1.0f,0.042154f,0.13f,
  0.0f,-1.0f,1.0f,0.0f,0.0f,1.0f,0.25f,0.01f,
  -0.866025f,-0.5f,-1.0f,-1.0f,-0.0f,-0.0f,0.333333f,0.5f,
  -0.866025f,-0.5f,1.0f,-1.0f,-0.0f,-0.0f,0.333333f,1.0f,
  -0.866025f,0.5f,1.0f,-1.0f,-0.0f,-0.0f,0.166667f,1.0f,
  -0.866025f,0.5f,-1.0f,-1.0f,-0.0f,0.0f,0.166667f,0.5f,
  -0.866025f,0.5f,-1.0f,-0.5f,0.866025f,0.0f,0.166667f,0.5f,
  -0.866025f,0.5f,1.0f,-0.5f,0.866025f,0.0f,0.166667f,1.0f,
  0.0f,1.0f,1.0f,-0.5f,0.866025f,0.0f,-0.0f,1.0f,
  0.0f,1.0f,-1.0f,-0.5f,0.866025f,0.0f,-0.0f,0.5f,
  -0.866025f,0.5f,-1.0f,-0.0f,0.0f,-1.0f,0.542154f,0.37f,
  0.0f,1.0f,-1.0f,-0.0f,0.0f,-1.0f,0.75f,0.49f,
  0.866025f,0.5f,-1.0f,-0.0f,0.0f,-1.0f,0.957846f,0.37f,
  0.866025f,-0.5f,-1.0f,0.0f,0.0f,-1.0f,0.957846f,0.13f,
  0.0f,-1.0f,-1.0f,0.0f,0.0f,-1.0f,0.75f,0.01f,
  -0.866025f,-0.5f,-1.0f,0.0f,0.0f,-1.0f,0.542154f,0.13f,
  2.95681f,2.34468f,-1.60224f,0.5f,0.866025f,0.0f,1.0f,0.5f,
  2.95681f,2.34468f,1.73109f,0.5f,0.866025f,0.0f,1.0f,1.0f,
  3.82283f,1.84468f,1.73109f,0.5f,0.866025f,0.0f,0.833333f,1.0f,
  3.82283f,1.84468f,-1.60224f,0.5f,0.866025f,-0.0f,0.833333f,0.5f,
  3.82283f,1.84468f,-1.60224f,1.0f,-0.0f,0.0f,0.833333f,0.5f,
  3.82283f,1.84468f,1.73109f,1.0f,-0.0f,0.0f,0.833333f,1.0f,
  3.82283f,0.844677f,1.73109f,1.0f,-0.0f,0.0f,0.666667f,1.0f,
  3.82283f,0.844677f,-1.60224f,1.0f,-0.0f,0.0f,0.666667f,0.5f,
  3.82283f,0.844677f,-1.60224f,0.5f,-0.866025f,0.0f,0.666667f,0.5f,
  3.82283f,0.844677f,1.73109f,0.5f,-0.866025f,0.0f,0.666667f,1.0f,
  2.95681f,0.344677f,1.73109f,0.5f,-0.866025f,0.0f,0.5f,1.0f,
  2.95681f,0.344677f,-1.60224f,0.5f,-0.866025f,0.0f,0.5f,0.5f,
  2.95681f,0.344677f,-1.60224f,-0.5f,-0.866025f,-0.0f,0.5f,0.5f,
  2.95681f,0.344677f,1.73109f,-0.5f,-0.866025f,-0.0f,0.5f,1.0f,
  2.09078f,0.844677f,1.73109f,-0.5f,-0.866025f,-0.0f,0.333333f,1.0f,
  2.09078f,0.844677f,-1.60224f,-0.5f,-0.866025f,0.0f,0.333333f,0.5f,
  3.82283f,0.844677f,1.73109f,0.0f,0.0f,1.0f,0.457846f,0.13f,
  3.82283f,1.84468f,1.73109f,0.0f,0.0f,1.0f,0.457846f,0.37f,
  2.95681f,2.34468f,1.73109f,0.0f,0.0f,1.0f,0.25f,0.49f,
  2.09078f,1.84468f,1.73109f,0.0f,0.0f,1.0f,0.042154f,0.37f,
  2.09078f,0.844677f,1.73109f,0.0f,0.0f,1.0f,0.042154f,0.13f,
  2.95681f,0.344677f,1.73109f,0.0f,0.0f,1.0f,0.25f,0.01f,
  2.09078f,0.844677f,-1.60224f,-1.0f,-0.0f,-0.0f,0.333333f,0.5f,
  2.09078f,0.844677f,1.73109f,-1.0f,-0.0f,-0.0f,0.333333f,1.0f,
  2.09078f,1.84468f,1.73109f,-1.0f,-0.0f,-0.0f,0.166667f,1.0f,
  2.09078f,1.84468f,-1.60224f,-1.0f,-0.0f,0.0f,0.166667f,0.5f,
  2.09078f,1.84468f,-1.60224f,-0.5f,0.866025f,0.0f,0.166667f,0.5f,
  2.09078f,1.84468f,1.73109f,-0.5f,0.866025f,0.0f,0.166667f,1.0f,
  2.95681f,2.34468f,1.73109f,-0.5f,0.866025f,0.0f,-0.0f,1.0f,
  2.95681f,2.34468f,-1.60224f,-0.5f,0.866025f,0.0f,-0.0f,0.5f,
  2.09078f,1.84468f,-1.60224f,-0.0f,0.0f,-1.0f,0.542154f,0.37f,
  2.95681f,2.34468f,-1.60224f,-0.0f,0.0f,-1.0f,0.75f,0.49f,
  3.82283f,1.84468f,-1.60224f,-0.0f,0.0f,-1.0f,0.957846f,0.37f,
  3.82283f,0.844677f,-1.60224f,0.0f,0.0f,-1.0f,0.957846f,0.13f,
  2.95681f,0.344677f,-1.60224f,0.0f,0.0f,-1.0f,0.75f,0.01f,
  2.09078f,0.844677f,-1.60224f,0.0f,0.0f,-1.0f,0.542154f,0.13f,
  2.95681f,-3.26292f,-1.60224f,0.5f,0.866025f,0.0f,1.0f,0.5f,
  2.95681f,-3.26292f,1.73109f,0.5f,0.866025f,0.0f,1.0f,1.0f,
  3.82283f,-3.76292f,1.73109f,0.5f,0.866025f,0.0f,0.833333f,1.0f,
  3.82283f,-3.76292f,-1.60224f,0.5f,0.866025f,-0.0f,0.833333f,0.5f,
  3.82283f,-3.76292f,-1.60224f,1.0f,-0.0f,0.0f,0.833333f,0.5f,
  3.82283f,-3.76292f,1.73109f,1.0f,-0.0f,0.0f,0.833333f,1.0f,
  3.82283f,-4.76292f,1.73109f,1.0f,-0.0f,0.0f,0.666667f,1.0f,
  3.82283f,-4.76292f,-1.60224f,1.0f,-0.0f,0.0f,0.666667f,0.5f,
  3.82283f,-4.76292f,-1.60224f,0.5f,-0.866025f,0.0f,0.666667f,0.5f,
  3.82283f,-4.76292f,1.73109f,0.5f,-0.866025f,0.0f,0.666667f,1.0f,
  2.95681f,-5.26292f,1.73109f,0.5f,-0.866025f,0.0f,0.5f,1.0f,
  2.95681f,-5.26292f,-1.60224f,0.5f,-0.866025f,0.0f,0.5f,0.5f,
  2.95681f,-5.26292f,-1.60224f,-0.5f,-0.866025f,-0.0f,0.5f,0.5f,
  2.95681f,-5.26292f,1.73109f,-0.5f,-0.866025f,-0.0f,0.5f,1.0f,
  2.09078f,-4.76292f,1.73109f,-0.5f,-0.866025f,-0.0f,0.333333f,1.0f,
  2.09078f,-4.76292f,-1.60224f,-0.5f,-0.866025f,0.0f,0.333333f,0.5f,
  3.82283f,-4.76292f,1.73109f,0.0f,0.0f,1.0f,0.457846f,0.13f,
  3.82283f,-3.76292f,1.73109f,0.0f,0.0f,1.0f,0.457846f,0.37f,
  2.95681f,-3.26292f,1.73109f,0.0f,0.0f,1.0f,0.25f,0.49f,
  2.09078f,-3.76292f,1.73109f,0.0f,0.0f,1.0f,0.042154f,0.37f,
  2.09078f,-4.76292f,1.73109f,0.0f,0.0f,1.0f,0.042154f,0.13f,
  2.95681f,-5.26292f,1.73109f,0.0f,0.0f,1.0f,0.25f,0.01f,
  2.09078f,-4.76292f,-1.60224f,-1.0f,-0.0f,-0.0f,0.333333f,0.5f,
  2.09078f,-4.76292f,1.73109f,-1.0f,-0.0f,-0.0f,0.333333f,1.0f,
  2.09078f,-3.76292f,1.73109f,-1.0f,-0.0f,-0.0f,0.166667f,1.0f,
  2.09078f,-3.76292f,-1.60224f,-1.0f,-0.0f,0.0f,0.166667f,0.5f,
  2.09078f,-3.76292f,-1.60224f,-0.5f,0.866025f,0.0f,0.166667f,0.5f,
  2.09078f,-3.76292f,1.73109f,-0.5f,0.866025f,0.0f,0.166667f,1.0f,
  2.95681f,-3.26292f,1.73109f,-0.5f,0.866025f,0.0f,-0.0f,1.0f,
  2.95681f,-3.26292f,-1.60224f,-0.5f,0.866025f,0.0f,-0.0f,0.5f,
  2.09078f,-3.76292f,-1.60224f,-0.0f,0.0f,-1.0f,0.542154f,0.37f,
  2.95681f,-3.26292f,-1.60224f,-0.0f,0.0f,-1.0f,0.75f,0.49f,
  3.82283f,-3.76292f,-1.60224f,-0.0f,0.0f,-1.0f,0.957846f,0.37f,
  3.82283f,-4.76292f,-1.60224f,0.0f,0.0f,-1.0f,0.957846f,0.13f,
  2.95681f,-5.26292f,-1.60224f,0.0f,0.0f,-1.0f,0.75f,0.01f,
  2.09078f,-4.76292f,-1.60224f,0.0f,0.0f,-1.0f,0.542154f,0.13f,
  1.38114f,2.11665f,1.37364f,0.214116f,-0.0f,0.976808f,1.0f,0.5f,
  1.38114f,-4.9071f,1.37364f,0.214116f,-0.0f,0.976808f,1.0f,1.0f,
  2.24717f,-4.9071f,1.1838f,0.214116f,-0.0f,0.976808f,0.833333f,1.0f,
  2.24717f,2.11665f,1.1838f,0.214116f,-0.0f,0.976808f,0.833333f,0.5f,
  2.24717f,2.11665f,1.1838f,1.0f,0.0f,0.0f,0.833333f,0.5f,
  2.24717f,-4.9071f,1.1838f,1.0f,0.0f,0.0f,0.833333f,1.0f,
  2.24717f,-4.9071f,0.804139f,1.0f,0.0f,0.0f,0.666667f,1.0f,
  2.24717f,2.11665f,0.804139f,1.0f,0.0f,-0.0f,0.666667f,0.5f,
  2.24717f,2.11665f,0.804139f,0.214116f,0.0f,-0.976808f,0.666667f,0.5f,
  2.24717f,-4.9071f,0.804139f,0.214116f,0.0f,-0.976808f,0.666667f,1.0f,
  1.38114f,-4.9071f,0.614307f,0.214116f,0.0f,-0.976808f,0.5f,1.0f,
  1.38114f,2.11665f,0.614307f,0.214116f,0.0f,-0.976808f,0.5f,0.5f,
  1.38114f,2.11665f,0.614307f,-0.214116f,0.0f,-0.976808f,0.5f,0.5f,
  1.38114f,-4.9071f,0.614307f,-0.214116f,0.0f,-0.976808f,0.5f,1.0f,
  0.515115f,-4.9071f,0.804139f,-0.214116f,0.0f,-0.976808f,0.333333f,1.0f,
  0.515115f,2.11665f,0.804139f,-0.214116f,0.0f,-0.976808f,0.333333f,0.5f,
  2.24717f,-4.9071f,0.804139f,0.0f,-1.0f,0.0f,0.457846f,0.13f,
  2.24717f,-4.9071f,1.1838f,0.0f,-1.0f,0.0f,0.457846f,0.37f,
  1.38114f,-4.9071f,1.37364f,0.0f,-1.0f,0.0f,0.25f,0.49f,
  0.515115f,-4.9071f,1.1838f,0.0f,-1.0f,0.0f,0.042154f,0.37f,
  0.515115f,-4.9071f,0.804139f,0.0f,-1.0f,0.0f,0.042154f,0.13f,
  1.38114f,-4.9071f,0.614307f,-0.0f,-1.0f,0.0f,0.25f,0.01f,
  0.515115f,2.11665f,0.804139f,-1.0f,0.0f,0.0f,0.333333f,0.5f,
  0.515115f,-4.9071f,0.804139f,-1.0f,0.0f,0.0f,0.333333f,1.0f,
  0.515115f,-4.9071f,1.1838f,-1.0f,0.0f,0.0f,0.166667f,1.0f,
  0.515115f,2.11665f,1.1838f,-1.0f,0.0f,-0.0f,0.166667f,0.5f,
  0.515115f,2.11665f,1.1838f,-0.214116f,-0.0f,0.976808f,0.166667f,0.5f,
  0.515115f,-4.9071f,1.1838f,-0.214116f,-0.0f,0.976808f,0.166667f,1.0f,
  1.38114f,-4.9071f,1.37364f,-0.214116f,-0.0f,0.976808f,-0.0f,1.0f,
  1.38114f,2.11665f,1.37364f,-0.214116f,-0.0f,0.976808f,-0.0f,0.5f,
  0.515115f,2.11665f,1.1838f,0.0f,1.0f,0.0f,0.542154f,0.37f,
  1.38114f,2.11665f,1.37364f,0.0f,1.0f,0.0f,0.75f,0.49f,
  2.24717f,2.11665f,1.1838f,0.0f,1.0f,0.0f,0.957846f,0.37f,
  2.24717f,2.11665f,0.804139f,0.0f,1.0f,0.0f,0.957846f,0.13f,
  1.38114f,2.11665f,0.614307f,0.0f,1.0f,0.0f,0.75f,0.01f,
  0.515115f,2.11665f,0.804139f,0.0f,1.0f,0.0f,0.542154f,0.13f,
  1.38114f,2.22804f,-0.610323f,0.214116f,-0.0f,0.976808f,1.0f,0.5f,
  1.38114f,-4.79571f,-0.610323f,0.214116f,-0.0f,0.976808f,1.0f,1.0f,
  2.24717f,-4.79571f,-0.800155f,0.214116f,-0.0f,0.976808f,0.833333f,1.0f,
  2.24717f,2.22804f,-0.800155f,0.214116f,-0.0f,0.976808f,0.833333f,0.5f,
  2.24717f,2.22804f,-0.800155f,1.0f,0.0f,0.0f,0.833333f,0.5f,
  2.24717f,-4.79571f,-0.800155f,1.0f,0.0f,0.0f,0.833333f,1.0f,
  2.24717f,-4.79571f,-1.17982f,1.0f,0.0f,0.0f,0.666667f,1.0f,
  2.24717f,2.22804f,-1.17982f,1.0f,0.0f,-0.0f,0.666667f,0.5f,
  2.24717f,2.22804f,-1.17982f,0.214116f,0.0f,-0.976808f,0.666667f,0.5f,
  2.24717f,-4.79571f,-1.17982f,0.214116f,0.0f,-0.976808f,0.666667f,1.0f,
  1.38114f,-4.79571f,-1.36965f,0.214116f,0.0f,-0.976808f,0.5f,1.0f,
  1.38114f,2.22804f,-1.36965f,0.214116f,0.0f,-0.976808f,0.5f,0.5f,
  1.38114f,2.22804f,-1.36965f,-0.214116f,0.0f,-0.976808f,0.5f,0.5f,
  1.38114f,-4.79571f,-1.36965f,-0.214116f,0.0f,-0.976808f,0.5f,1.0f,
  0.515115f,-4.79571f,-1.17982f,-0.214116f,0.0f,-0.976808f,0.333333f,1.0f,
  0.515115f,2.22804f,-1.17982f,-0.214116f,0.0f,-0.976808f,0.333333f,0.5f,
  2.24717f,-4.79571f,-1.17982f,0.0f,-1.0f,0.0f,0.457846f,0.13f,
  2.24717f,-4.79571f,-0.800155f,0.0f,-1.0f,0.0f,0.457846f,0.37f,
  1.38114f,-4.79571f,-0.610323f,0.0f,-1.0f,0.0f,0.25f,0.49f,
  0.515115f,-4.79571f,-0.800155f,0.0f,-1.0f,0.0f,0.042154f,0.37f,
  0.515115f,-4.79571f,-1.17982f,0.0f,-1.0f,0.0f,0.042154f,0.13f,
  1.38114f,-4.79571f,-1.36965f,-0.0f,-1.0f,0.0f,0.25f,0.01f,
  0.515115f,2.22804f,-1.17982f,-1.0f,0.0f,0.0f,0.333333f,0.5f,
  0.515115f,-4.79571f,-1.17982f,-1.0f,0.0f,0.0f,0.333333f,1.0f,
  0.515115f,-4.79571f,-0.800155f,-1.0f,0.0f,0.0f,0.166667f,1.0f,
  0.515115f,2.22804f,-0.800155f,-1.0f,0.0f,-0.0f,0.166667f,0.5f,
  0.515115f,2.22804f,-0.800155f,-0.214116f,-0.0f,0.976808f,0.166667f,0.5f,
  0.515115f,-4.79571f,-0.800155f,-0.214116f,-0.0f,0.976808f,0.166667f,1.0f,
  1.38114f,-4.79571f,-0.610323f,-0.214116f,-0.0f,0.976808f,-0.0f,1.0f,
  1.38114f,2.22804f,-0.610323f,-0.214116f,-0.0f,0.976808f,-0.0f,0.5f,
  0.515115f,2.22804f,-0.800155f,0.0f,1.0f,0.0f,0.542154f,0.37f,
  1.38114f,2.22804f,-0.610323f,0.0f,1.0f,0.0f,0.75f,0.49f,
  2.24717f,2.22804f,-0.800155f,0.0f,1.0f,0.0f,0.957846f,0.37f,
  2.24717f,2.22804f,-1.17982f,0.0f,1.0f,0.0f,0.957846f,0.13f,
  1.38114f,2.22804f,-1.36965f,0.0f,1.0f,0.0f,0.75f,0.01f,
  0.515115f,2.22804f,-1.17982f,0.0f,1.0f,0.0f,0.542154f,0.13f,
  -5.15341e-07f,-1.87135f,-1.0f,0.5f,0.866025f,0.0f,1.0f,0.5f,
  -5.15341e-07f,-1.87135f,1.0f,0.5f,0.866025f,0.0f,1.0f,1.0f,
  0.866025f,-2.37135f,1.0f,0.5f,0.866025f,0.0f,0.833333f,1.0f,
  0.866025f,-2.37135f,-1.0f,0.5f,0.866025f,-0.0f,0.833333f,0.5f,
  0.866025f,-2.37135f,-1.0f,1.0f,-0.0f,0.0f,0.833333f,0.5f,
  0.866025f,-2.37135f,1.0f,1.0f,-0.0f,0.0f,0.833333f,1.0f,
  0.866025f,-3.37135f,1.0f,1.0f,-0.0f,0.0f,0.666667f,1.0f,
  0.866025f,-3.37135f,-1.0f,1.0f,-0.0f,0.0f,0.666667f,0.5f,
  0.866025f,-3.37135f,-1.0f,0.5f,-0.866025f,0.0f,0.666667f,0.5f,
  0.866025f,-3.37135f,1.0f,0.5f,-0.866025f,0.0f,0.666667f,1.0f,
  -5.15341e-07f,-3.87135f,1.0f,0.5f,-0.866025f,0.0f,0.5f,1.0f,
  -5.15341e-07f,-3.87135f,-1.0f,0.5f,-0.866025f,0.0f,0.5f,0.5f,
  -5.15341e-07f,-3.87135f,-1.0f,-0.5f,-0.866025f,-0.0f,0.5f,0.5f,
  -5.15341e-07f,-3.87135f,1.0f,-0.5f,-0.866025f,-0.0f,0.5f,1.0f,
  -0.866026f,-3.37135f,1.0f,-0.5f,-0.866025f,-0.0f,0.333333f,1.0f,
  -0.866026f,-3.37135f,-1.0f,-0.5f,-0.866025f,0.0f,0.333333f,0.5f,
  0.866025f,-3.37135f,1.0f,0.0f,0.0f,1.0f,0.457846f,0.13f,
  0.866025f,-2.37135f,1.0f,0.0f,0.0f,1.0f,0.457846f,0.37f,
  -5.15341e-07f,-1.87135f,1.0f,0.0f,0.0f,1.0f,0.25f,0.49f,
  -0.866026f,-2.37135f,1.0f,0.0f,0.0f,1.0f,0.042154f,0.37f,
  -0.866026f,-3.37135f,1.0f,0.0f,0.0f,1.0f,0.042154f,0.13f,
  -5.15341e-07f,-3.87135f,1.0f,0.0f,0.0f,1.0f,0.25f,0.01f,
  -0.866026f,-3.37135f,-1.0f,-1.0f,-0.0f,-0.0f,0.333333f,0.5f,
  -0.866026f,-3.37135f,1.0f,-1.0f,-0.0f,-0.0f,0.333333f,1.0f,
  -0.866026f,-2.37135f,1.0f,-1.0f,-0.0f,-0.0f,0.166667f,1.0f,
  -0.866026f,-2.37135f,-1.0f,-1.0f,-0.0f,0.0f,0.166667f,0.5f,
  -0.866026f,-2.37135f,-1.0f,-0.5f,0.866025f,0.0f,0.166667f,0.5f,
  -0.866026f,-2.37135f,1.0f,-0.5f,0.866025f,0.0f,0.166667f,1.0f,
  -5.15341e-07f,-1.87135f,1.0f,-0.5f,0.866025f,0.0f,-0.0f,1.0f,
  -5.15341e-07f,-1.87135f,-1.0f,-0.5f,0.866025f,0.0f,-0.0f,0.5f,
  -0.866026f,-2.37135f,-1.0f,-0.0f,0.0f,-1.0f,0.542154f,0.37f,
  -5.15341e-07f,-1.87135f,-1.0f,-0.0f,0.0f,-1.0f,0.75f,0.49f,
  0.866025f,-2.37135f,-1.0f,-0.0f,0.0f,-1.0f,0.957846f,0.37f,
  0.866025f,-3.37135f,-1.0f,0.0f,0.0f,-1.0f,0.957846f,0.13f,
  -5.15341e-07f,-3.87135f,-1.0f,0.0f,0.0f,-1.0f,0.75f,0.01f,
  -0.866026f,-3.37135f,-1.0f,0.0f,0.0f,-1.0f,0.542154f,0.13f,
}; // end fire_bigVertices

const unsigned fire_bigTriangles[] = {
  0, 1, 2,
  0, 2, 3,
  4, 5, 6,
  4, 6, 7,
  8, 9, 10,
  8, 10, 11,
  12, 13, 14,
  12, 14, 15,
  16, 17, 18,
  18, 19, 20,
  20, 21, 16,
  18, 20, 16,
  22, 23, 24,
  22, 24, 25,
  26, 27, 28,
  26, 28, 29,
  30, 31, 32,
  32, 33, 34,
  34, 35, 30,
  32, 34, 30,
  36, 37, 38,
  36, 38, 39,
  40, 41, 42,
  40, 42, 43,
  44, 45, 46,
  44, 46, 47,
  48, 49, 50,
  48, 50, 51,
  52, 53, 54,
  54, 55, 56,
  56, 57, 52,
  54, 56, 52,
  58, 59, 60,
  58, 60, 61,
  62, 63, 64,
  62, 64, 65,
  66, 67, 68,
  68, 69, 70,
  70, 71, 66,
  68, 70, 66,
  72, 73, 74,
  72, 74, 75,
  76, 77, 78,
  76, 78, 79,
  80, 81, 82,
  80, 82, 83,
  84, 85, 86,
  84, 86, 87,
  88, 89, 90,
  90, 91, 92,
  92, 93, 88,
  90, 92, 88,
  94, 95, 96,
  94, 96, 97,
  98, 99, 100,
  98, 100, 101,
  102, 103, 104,
  104, 105, 106,
  106, 107, 102,
  104, 106, 102,
  108, 109, 110,
  108, 110, 111,
  112, 113, 114,
  112, 114, 115,
  116, 117, 118,
  116, 118, 119,
  120, 121, 122,
  120, 122, 123,
  124, 125, 126,
  126, 127, 128,
  128, 129, 124,
  126, 128, 124,
  130, 131, 132,
  130, 132, 133,
  134, 135, 136,
  134, 136, 137,
  138, 139, 140,
  140, 141, 142,
  142, 143, 138,
  140, 142, 138,
  144, 145, 146,
  144, 146, 147,
  148, 149, 150,
  148, 150, 151,
  152, 153, 154,
  152, 154, 155,
  156, 157, 158,
  156, 158, 159,
  160, 161, 162,
  162, 163, 164,
  164, 165, 160,
  162, 164, 160,
  166, 167, 168,
  166, 168, 169,
  170, 171, 172,
  170, 172, 173,
  174, 175, 176,
  176, 177, 178,
  178, 179, 174,
  176, 178, 174,
  180, 181, 182,
  180, 182, 183,
  184, 185, 186,
  184, 186, 187,
  188, 189, 190,
  188, 190, 191,
  192, 193, 194,
  192, 194, 195,
  196, 197, 198,
  198, 199, 200,
  200, 201, 196,
  198, 200, 196,
  202, 203, 204,
  202, 204, 205,
  206, 207, 208,
  206, 208, 209,
  210, 211, 212,
  212, 213, 214,
  214, 215, 210,
  212, 214, 210,
}; // end fire_bigTriangles

