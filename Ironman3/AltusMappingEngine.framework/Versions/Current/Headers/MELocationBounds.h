//  Copyright (c) 2015 BA3, LLC. All rights reserved.
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
	/**
	 Represents a geographic bounds.
	 */
	typedef struct MELocationBounds {
		double minX;
		double minY;
		double maxX;
		double maxY;
	} MELocationBounds;
	
	/**
	 Make a MELocationBounds struct and populate its members.
	 */
	MELocationBounds MELocationBoundsMake(double minX, double minY,
										  double maxX, double maxY);
	
#ifdef __cplusplus
}
#endif

