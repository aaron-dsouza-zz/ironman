//  Copyright © 2016 BA3, LLC. All rights reserved.
#pragma once
#ifdef __cplusplus
extern "C" {
#endif

	/**
	Represents a model vertex.
	*/
	typedef struct MEModelVertex{
		float x;   ///< position x
		float y;   ///< position y
		float z;   ///< position z
		float nx;  ///< normal x
		float ny;  ///< normal y
		float nz;  ///< normal z
		float u;   ///< texcoord u
		float v;   ///< texcoord v
	} MEModelVertex;

	/**
	Make a MEModelVertex struct and populate its members.
	*/
	MEModelVertex MEModelVertexMake(float x, float y, float z,
									float nx, float ny, float nz,
									float u, float v);
	

#ifdef __cplusplus
}
#endif

