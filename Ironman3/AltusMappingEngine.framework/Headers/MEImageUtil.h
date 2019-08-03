//  Copyright (c) 2012 BA3, LLC. All rights reserved.

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include <QuartzCore/QuartzCore.h>

/**
 Utility class for working with images.
 */
@interface MEImageUtil : NSObject

+ (CGContextRef) createBitmapContextSuitableForView:(UIView*) uiView;

/**Creates a context of the specified dimensions that is compatible with the rendering engine.*/
+ (CGContextRef) createContext:(CGSize) size
                         scale:(float) scale
                      flippedY:(BOOL) flippedY;

/**Destroys a context created by MEImageUtil*/
+ (void) destroyContext:(CGContextRef) context;

/**Converts a context into a UIImage.*/
+ (UIImage *) createImageFromContext:(CGContextRef) context;

/**Converts a UIView into a UIImage.*/
+ (UIImage *) createImageFromView:(UIView*) uiView;

/**Copies and scales RGBA bitmap data from a UIImage to specified memory returning the new physical size in pixels in resultantWidht and resultantHeight, optionally flipping the Y. You must free the memory
 with a call to free().*/
+ (void *) bitmapFromImage:(UIImage *)image
                  flippedY:(BOOL) flippedY
                     scale:(float) scale
            resultantWidth:(unsigned int*) resultantWidth
           resultantHeight:(unsigned int*) resultantHeight;

/**Copies and scales RGBA bitmap data from a UIImage to specified memory returning the new physical size specified by targetWidth and targetHeight in pixels, optionally flipping the Y. You must free the memory
 with a call to free().*/
+ (void *) bitmapFromImage:(UIImage *)image
                  flippedY:(BOOL) flippedY
            targetWidth:(unsigned int) targetWidth
           targetHeight:(unsigned int) targetHeight;

/**Reads data from a UIImage, malloc's memory at imageData and copies it there in RGBA8888 format. Stores image attributes in imageWidth, imageHeight, and imageDataLenght*/
+ (void) imageToRGBA8888:(UIImage *)image
               imageData:(void**) imageData
              imageWidth:(unsigned int*) imageWidth
             imageHeight:(unsigned int*) imageHeight
         imageDataLength:(unsigned int*) imageDataLength;

+ (UIImage*) imageFromRGBA8888:(void**) imageData
					imageWidth:(unsigned int) imageWidth
				   imageHeight:(unsigned int) imageHeight
			   imageDataLength:(unsigned int) imageDataLength
						 scale:(float) scale;

/**Returns YES if the RGBA8888 memory block pointed to by imageData contains any alpha values < 255.*/
+ (BOOL) containsClearPixels:(void*) imageData
			 imageDataLength:(unsigned int) imageDataLength;

/**Returns a UIColor for r g b a bytes.*/
+ (UIColor*) makeColor:(int) r g:(int) g b:(int) b a:(int) a;

/**Returns a 1x1 pixel UIImage filled with the specified color.
@param color The color to make the image.
*/
+ (UIImage *)imageWithColor:(UIColor *)color;

@end
