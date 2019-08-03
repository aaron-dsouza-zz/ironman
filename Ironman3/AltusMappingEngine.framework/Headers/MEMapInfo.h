//  Copyright (c) 2012 BA3, LLC. All rights reserved.
#import <Foundation/Foundation.h>
#import "MEProtocols.h"
#import "METileInfo.h"
#import "MEEnumerations.h"

//////////////////////////////////////////////////
//Forward declarations
@class MEMapViewController;
@class METileProvider;

/**Base class object that describes a map layer and options for it. Used when adding a map, virtual map or inquiring about map. There are several sub-classes of MEMapInfo. When adding a map, you create an MEMapInfo object of the appropriate type, populate its properties and then call MEMapViewController addMapUsingMapInfo passing in the MEMapInfo object.
 
 There are several possible map types, see MEMapType.
 
 When adding a map, it is best to use the appropriate MEMapInfo-derived object type:
 
 - MEMBTilesMapInfo
 - MEDynamicMarkerMapInfo
 - MEVirtualMarkerMapInfo
 - MEMarkerMapInfo
 - MEVectorMapInfo
 - MEVirtualMapInfo
 - MEAnimatedVirtualMapInfo
 - METerrain3DMapInfo
 - MEModel3DMapInfo
 
 Different map types can have different content types and loading strategies. Please refer to:
 
 - MEMapContentType
 - MEloadingStrategy
 - MEMarkerImageLoadingStrategy
 */
@interface MEMapInfo : NSObject <NSCopying>

/** Map type.
 @see MEMapType*/
@property (assign) MEMapType mapType;

/** Associated filename. */
@property (retain) NSString* sqliteFileName;

/** Table name prefix. */
@property (retain) NSString* tableNamePrefix;

/**Map loading strategy type.*/
@property (assign) MEloadingStrategy loadingStrategy;

/**Unique name of map.*/
@property (retain) NSString* name;

/**They layer order of this map, higher means higher in the stack.*/
@property (assign) unsigned int zOrder;

/**Maximum detail level of the map. (Specified for in-memory marker maps and virtual maps).*/
@property (assign) unsigned int maxLevel;

/**Specifies the relative priority of this map layer. Please read see MEMapViewController's setMapPriority function for an explanation of priority.*/
@property (assign) int priority;

/**0 to 1 value for the map alpha. 0 is invisible, 1 is opaque.*/
@property (assign) double alpha;

/**Whether or not the map is vibile.*/
@property (assign) BOOL isVisible;

/**Whether or not to compress textures for this map to 2 byte formats.*/
@property (assign) BOOL compressTextures;

/**The minimum longitude for this map.*/
@property (assign) double minX;

/**The minimum latitude for this map.*/
@property (assign) double minY;

/**The maximum longitude for this map.*/
@property (assign) double maxX;

/**The maximum latitude for this map.*/
@property (assign) double maxY;

/**Specifies number of pixels that border each tile.*/
@property (assign) unsigned int borderPixelCount;

/**Specifies the name of the pre-loaded default tile to render while tiles are being loaded or to use when a tile is not available. */
@property (retain) NSString* defaultTileName;

/**Returns a human readable map type description for the specified map type.*/
+(NSString*) mapTypeDescription:(MEMapType) mapType;

@end

//////////////////////////////////////////////////
/**Describes an MBTiles map.*/
@interface MEMBTilesMapInfo : MEMapInfo

/**The type of image tiles in the map.*/
@property (assign) MEImageDataType imageDataType;

@end

////////////////////////////////////////////////////////////////////////
/**Describes a dynamic marker map. Use dynamic marker maps when you need to frequently update a marker image, rotation, and/or position.
 */
@interface MEDynamicMarkerMapInfo : MEMapInfo

/**Delegate to receive hit testing notifications.*/
@property (retain) id<MEDynamicMarkerMapDelegate> meDynamicMarkerMapDelegate;

/**Controls whether the engine performs touch-point hit testing against the markers. Defaults to YES.*/
@property (assign) BOOL hitTestingEnabled;

/**Specifies whether markers fade in.*/
@property (assign) BOOL fadeEnabled;

/**Amount of time in seconds that markers fade in.*/
@property (assign) double fadeInTime;

/**Amount of time in seconds that markers fade out.*/
@property (assign) double fadeOutTime;

@end

//////////////////////////////////////////////////
/**Describes a virtual marker map.*/
@interface MEVirtualMarkerMapInfo : MEDynamicMarkerMapInfo

/**Tile provider that will serve up tiles for this virtual map.*/
@property (retain) METileProvider* meTileProvider;

@end

//////////////////////////////////////////////////
/**Describes a marker map.*/
@interface MEMarkerMapInfo : MEMapInfo

/**For marker maps, the marker map delegate that will provide marker images.*/
@property (retain) id<MEMarkerMapDelegate> meMarkerMapDelegate;

/**For marker maps, the array of markers to add.*/
@property (retain) NSArray* markers;

/**Cluster distance to use for markers.*/
@property (assign) CGFloat clusterDistance;

/**Controls how the engine requests marker images. Defaults to kAsynchronousMarkerImageLoading. Note: frame hitching can occur if you use kMarkerImageLoadingAsynchronous for a large set of markers. Use asynchronous loading when possible.*/
@property (assign) MEMarkerImageLoadingStrategy markerImageLoadingStrategy;

/**Controls whether the engine performs touch-point hit testing against the markers. Defaults to YES.*/
@property (assign) BOOL hitTestingEnabled;

/**Specifies whether markers fade in.*/
@property (assign) BOOL fadeEnabled;

/**Amount of time in seconds that markers fade in.*/
@property (assign) double fadeInTime;

/**Amount of time in seconds that markers fade out.*/
@property (assign) double fadeOutTime;

@end

//////////////////////////////////////////////////
/**Describes a vector map.*/
@interface MEVectorMapInfo : MEMapInfo

/**Optional delegate that will receive touch events on dynamic vector geometry objects.*/
@property (retain) id<MEVectorMapDelegate> meVectorMapDelegate;

/**Specifies if polygon hit detection is enabled for this map. If YES, the meVectrMapDelegate will receive notification of polygons that user taps on.*/
@property (assign) BOOL polygonHitDetectionEnabled;

@end

//////////////////////////////////////////////////
/**Describes a virtual map.*/
@interface MEVirtualMapInfo : MEMapInfo

/**Tile provider that will serve up tiles for this virtual map.*/
@property (retain) METileProvider* meTileProvider;

/**Specifies map content type.*/
@property (assign) MEMapContentType contentType;

@end

////////////////////////////////////////////////////
/**Describes a 3D terrain map.*/
@interface METerrain3DMapInfo : MEMapInfo
@end

////////////////////////////////////////////////////
/**Describes a dynamic 3D model map.  You can modify the position and rotation of models in this map.*/
@interface MEModel3DMapInfo : MEMapInfo
@end






