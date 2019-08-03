//  Copyright (c) 2013 BA3, LLC. All rights reserved.
#pragma once
//Enum for conveying image type in database
enum ImageDataType {
	IMAGEDATATYPE_PNG,
	IMAGEDATATYPE_JPG
};

#ifndef SWIG
/**Used to indicate the geometry layout of an MEAnimatedVectorReticle.*/
enum MEAnimatedVectorReticleStyle {
	kAnimatedVectorReticleInwardPointingWithCircle,
	kAnimatedVectorReticleInwardPointingWithoutCircle,
	kAnimatedVectorReticleOutwardPointingWithCircle,
	kAnimatedVectorReticleOutwardPointingWithoutCircle
};
#endif