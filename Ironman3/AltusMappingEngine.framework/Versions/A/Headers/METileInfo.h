//  Copyright (c) 2012 BA3, LLC. All rights reserved.
#import <CoreLocation/CoreLocation.h>
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "MEEnumerations.h"

//Forward-declarations
@class METileProviderRequest;

/**Describes an Altus tile.*/
@interface METile : NSObject

/**
 Init with an Altus 64-bit tile ID.
 */
-(id) initWithUID:(uint64_t) uid;

/**
 Init from a web mercator level/column/row ID (z/x/y). NOTE: This only works for mercator tiles, not tiles at the poles. See initWithLocation for a more general initializer.
 */
-(id) initWithLevel:(unsigned int) level
				  X:(unsigned int) X
				  Y:(unsigned int) Y;

/**
 Init with a geographic location and zoom level. Works for all tiles (mercator and polar).
 NOTE: Locations above 85.051128779806604N Latitude and below -85.051128779806604S Latitude will not have relevant Level, X, or Y properties.
 */
-(id) initWithLocation:(CLLocationCoordinate2D) location
				 level:(unsigned int) level;

/**Unqiue identifer for the tile.*/
@property (assign) uint64_t uid;
/**Region in which the tile appears.
 @see METileRegion*/
@property (assign) METileRegion region;
@property (assign) int level;
@property (assign) int x;
@property (assign) int y;
@property (assign) double minX;
@property (assign) double minY;
@property (assign) double maxX;
@property (assign) double maxY;
@end

/**
 Conveys information about virtual map tiles. When a virtual layer is added, these objects will be passed to the requestTile or requestTileAsync function of the tile provider. The tile provider populates the request with data and sets the response code.*/
@interface METileProviderRequest : NSObject

/**Information about the tile being requested.*/
@property (retain) METile* requestedTile;

/**Tile providers set this value based on how the mapping engine should interpret the tile METileProviderRequest it gets back form the tile provider.
 */
@property (assign) METileProviderResponse tileProviderResponse;

/**The internal map ID of the map that contains this tile.*/
@property (assign) size_t mapid;

/**Width of the tile image.*/
@property (assign) unsigned int width;

/**Height of the tile image.*/
@property (assign) unsigned int height;

/**In the case of setting pImageData, the lenght in bytes of the data.*/
@property (assign) unsigned int pImageDataLength;

/**If set, a pointer to a UIImage object to use as image data.*/
@property (retain) UIImage* uiImage;

/**If set, a pointer to an NSData object whose bytes represent compressed JPG or PNG image data. If set, you should also set the imageDataType appropriately.*/
@property (retain) NSData* nsImageData;

/**If set, a pointer to memory whose bytes represent compressed JPG or PNG image data. If set, you should also set the imageDataType appropriately.*/
@property (assign) void* pImageData;

/**If set, a pointer to a jpg or png file which the engine will load and decompress. The engine has native support decompressing png and jpg images very quickly.*/
@property (retain) NSString* fileName;

/** If set, specifies the name of a cached image to use. You may cache images by using MEMapViewController addCachedImage.*/
@property (retain) NSString* cachedImageName;

/** If setting nsImageData or pImageData, you should set this to the appropriate image data type.*/
@property (assign) MEImageDataType imageDataType;

/**If set, tells the mapping engine that every pixel of the tile is lit and has no semi-transparent pixels. This allows the engine to optimize storage and layering of the tile (i.e. no tile underneath this tile will be visible if it is opaque.*/
@property (assign) BOOL isOpaque;

/**If set to YES, this tile can be ejected and re-requested by calling refreshDirtyTiles on the MEMapViewController object. In order for this to work, you must provied a cached image name which would generally be some form of placeholder image.*/
@property (assign) BOOL isDirty;

/** If set to YES, tells the mapping engine to only render the tile if it cannot sample from a parent tile that was previously requested. This response can be used, for example, if you have an intermittent internet connection and cannot download the requested tile, but do not wish to show the user a placeholder tile if a parent tile can be sampled from.
 */
@property (assign) BOOL isProxy;

/**Initializer for METileProviderRequest*/
- (id) initWithUID:(uint64_t) uid
			 width:(int) width
			height:(int) height;

/**Used by the engine to write a pointer to internal engine data structures.*/
- (void) setPrivateData:(void*) pData;

/**Used by the engine to read internal engine data structures.*/
- (void*) getPrivateData;

@end

/**Represents the data the mapping engine returns when low-level inquiries are made about tiles being displayed or currently being requested.*/
@interface METileRequest : NSObject

/**The internal 64-bit ID of the tile.*/
@property (assign) uint64_t uid;

/**The internal map ID of the map that contains this tile.*/
@property (assign) size_t mapid;

@end




