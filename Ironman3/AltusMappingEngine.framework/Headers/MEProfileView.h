//  Copyright (c) 2015 BA3. All rights reserved.
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "MEMarker.h"
#import "MEMapInfo.h"
#import "MEProtocols.h"

@interface MEProfileViewMarkerResourceData : NSObject
@property (nonatomic, assign) CLLocationCoordinate2D location;
@property (nonatomic, assign) float percentageAlongPath;
@property (nonatomic, retain) MEMarker* marker;
-(id) initWithMarkerResource:(MEMarker*) markerResource percentageAlongPath:(float)percentage location:(CLLocationCoordinate2D)location;
@end

@interface MEProfileViewPointWithPercentage : NSObject
@property (nonatomic, assign) CLLocationCoordinate2D location;
@property (nonatomic, assign) float percentAlongPath;
-(id) initWithLocation:(CLLocationCoordinate2D)loc percentage:(float)percentage;
@end

@interface MEProfileViewExtrudedVectorData : NSObject
@property (nonatomic, retain) NSArray* points; /// An array of MEProfileViewPointWithPercentage
@property (nonatomic, assign) double minAltitude;
@property (nonatomic, assign) double maxAltitude;
@property (nonatomic, assign) uint styleId;
-(id) init;
-(id) initWithPoints:(NSArray*)points minAltitude:(double)minAltitude maxAltitude:(double)maxAltitude styleId:(uint)styldId;
@end

@interface MEProfileViewDesc : NSObject
@property (nonatomic, assign) int tileLevel;
@property (nonatomic, assign) int tileResolutionOffset;
@property (nonatomic, assign) bool preferSingleTileLevel;
@property (nonatomic, assign) bool useCache;
@property (nonatomic, assign) double widthOfPath; /// In meters.  Used for marker filtering.
-(id) init;
@end

@interface MESharedProfileViewDesc : MEProfileViewDesc
-(id) initWithPath:(NSArray*)path resultWidth:(uint)resultWidth;
@end

typedef enum MEProfileViewStatus
{
	kProfileViewStatusDone,						/// All tasks have completed.
	kProfileViewStatusMoreTasksCanStart,		/// There are tasks waiting to start.  Call Update again to start another task.
	kProfileViewStatusMaximumTasksExecuting,	/// The maximum number of tasks are executing.  When one finished, another can start.
	kProfileViewStatusTasksExecuting,			/// There are no tasks waiting to start and some tasks are unfinished.
} MEProfileViewStatus;

@interface MEProfileView : NSObject<METileLoader>
-(id) initWithScale:(float)scale;
-(void) cancelProfile:(uint) profileMetadata;
//** path is an array of CLLocations
-(void) generateProfile:(uint) profileMetadata usingMapInfo:(const MEVirtualMapInfo*)mapInfo path:(NSArray*)path resultWidth:(uint)resultWidth delegate:(id<MEProfileViewDelegate>)delegate desc:(MEProfileViewDesc*)desc;
-(void) generateProfile:(uint) profileMetadata usingMarkerMapInfo:(const MEVirtualMarkerMapInfo*)mapInfo path:(NSArray*)path resultWidth:(uint)resultWidth delegate:(id<MEProfileViewDelegate>)delegate desc:(MEProfileViewDesc*)desc;
-(MEProfileViewStatus) Update;



/**Called when a tile has been loaded through a virtual layer. Should only be called on the main thread, otherwise, a syncrhonous lock is used to ensure internal data structure integrity. Places the complete tile request back in the loading queue. See maxAsyncTileLoadsPerFrame.*/
- (void) tileLoadComplete:(METileProviderRequest*) meTileRequest;

/**Called when a tile has been loaded through a virtual layer. If loadImmediate is NO, places the tile request in the loading queue (see maxAsyncTileLoadsPerFrame), otherwise the loading is immediate. NOTE: if loadImmediate is YES you must only make the call from the main thread, otherwise you risk corrupting internal data structure. Warning: immediate loading too many tiles can cause frame hitching.*/
- (void) tileLoadComplete:(METileProviderRequest*) meTileRequest
			loadImmediate:(BOOL) loadImmediate;

/**Called by vector tile providers to supply geometry for a requested tile.*/
- (void) markerTileLoadComplete:(METileProviderRequest*) meTileRequest
					markerArray:(NSArray*) markerArray;

/**Called by vector tile providers to supply geometry for a requested tile.*/
- (void) vectorTileLoadComplete:(METileProviderRequest*) meTileRequest
				meGeometryGroup:(MEGeometryGroup*) meGeometryGroup;

/**Called by vector tile providers to supply binary geometry for a requested tile.*/
- (void) vectorTileLoadComplete:(METileProviderRequest*) meTileRequest
					   tileData:(NSData*)tileData;

/**Called by terrain tile providers to supply terrain height data.*/
- (void) terrainTileLoadComplete:(METileProviderRequest*) meTileRequest
					 terrainTile:(METerrainTile*)terrainTile;

- (void) weatherTileLoadComplete:(METileProviderRequest*) meTileRequest
						tileData:(NSData*)tileData;

/**Returns whether or not the engine considers the tile represented by meTileRequest to be required to satisfy the current view for any non-animated virtual map. This call will dispatched to the main queue if it is not made on the main queue.*/
-(BOOL) tileIsNeeded:(METileProviderRequest*) meTileRequest;

@property (atomic, assign) BOOL verboseMessagesEnabled;

@end
