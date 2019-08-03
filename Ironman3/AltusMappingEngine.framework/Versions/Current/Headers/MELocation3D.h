//  Copyright (c) 2015 BA3, LLC. All rights reserved.
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
	/**
	 Represents a gegraphic location and altitude above it.
	 */
	typedef struct MELocation3D {
		double latitude;
		double longitude;
		double altitude;
	} MELocation3D;
	
	
	/**
	 Make a MELocation3D struct and populate its members.
	 */
	MELocation3D MELocation3DMake(double latitude,
								  double longitude,
								  double altitude);
	
#ifdef __cplusplus
}
#endif

