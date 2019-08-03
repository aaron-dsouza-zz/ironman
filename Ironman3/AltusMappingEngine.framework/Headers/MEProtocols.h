//  Copyright (c) 2012 BA3, LLC. All rights reserved.
#pragma once
#import <MapKit/MapKit.h>
#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <CoreLocation/CoreLocation.h>

@class MEMapView;
@class MECameraPlacement;
@class METerrainTile;
@class METileProviderRequest;
@class MEGeometryGroup;

/////////////////////////////////////////////////////////////////////////////
/**The MEMapViewDelegate protocol defines a set of optional methods that you can use to receive map-related update messages. Because many map operations require the MEMapView class to load data asynchronously, the map view calls these methods to notify your application when specific operations complete. The map view also uses these methods to request annotation information.
 */
@protocol MEMapViewDelegate <NSObject>
@optional

/**Called when the user taps on a map.*/
- (void) mapView:(MEMapView *)mapView singleTapAt:(CGPoint) screenPoint withLocationCoordinate:(CLLocationCoordinate2D) locationCoordinate;

/**Called when the engine begins loading data for a map.*/
- (void) mapView:(MEMapView *)mapView willStartLoadingMap:(NSString*) mapName;

/**Called when the engine has finished loading map data -OR- all tile requests have been submitted to tile providers for virtual maps.*/
- (void) mapView:(MEMapView *)mapView didFinishLoadingMap:(NSString*) mapName;

/**Called when the engine begins clustering markers.*/
- (void) mapView:(MEMapView *)mapView beginClusteringMarkers:(NSString*) mapName;

/**Called when the engine finishes clustering markers.*/
- (void) mapView:(MEMapView *)mapView endClusteringMarkers:(NSString*) mapName;


/**Called when the user has started panning the view.*/
- (void) panBegan:(MEMapView *) mapView;

/**Called when the panning is completed.*/
- (void) panEnded:(MEMapView *) mapView;

/**Called when a pinch operation starts.*/
- (void) pinchBegan:(MEMapView *) mapView;

/**Called when a pinch operation ends.*/
- (void) pinchEnded:(MEMapView *) mapView;

/**Called to determine if the engine should receive the given gesture.*/
- (BOOL) meGestureRecognizer : (UIGestureRecognizer *) gestureRecognizer shouldReceiveTouch : (UITouch *) touch;

/**Called to determine if the given gesture recognizer should begin.*/
- (BOOL) meGestureRecognizerShouldBegin:(UIGestureRecognizer *)gestureRecognizer;

/**Called when the physical device scale changes.*/
- (void) deviceScaleChanged:(MEMapView *) mapView;

/**Called when the tile point size changes.*/
- (void) tileSizeChanged:(MEMapView *) mapView;

/**Camera position has changed since last draw*/
- (void) cameraMoved:(MEMapView *) mapView;

@end


@class MEMarker;

/**
 The MEMarkerMapDelegate protocol defines a set of methods that you can use to receive marker map related update messages. Implement this protocol when you add marker maps.
 */
@protocol MEMarkerMapDelegate<NSObject>

@optional
/**
 Called when the mapping engine needs information for a marker in a marker layer. When called, you should populate the provided MEMarkerInfo object. The engine will release the object, so you don't need to.
 @param mapView the MEMapView object that owns the marker.
 @param marker Object for you to populate that describes the marker.
 @param mapName The name of the map.
 */
- (void) mapView:(MEMapView*)mapView
	updateMarker:(MEMarker*) marker
		 mapName:(NSString*) mapName;

/**
 Called when a marker is tapped on. To handle multiple marker taps, see tapOnMarkers.
 */
- (void) tapOnMarker:(NSString*)metaData
		   onMapView:(MEMapView*)mapView
	   atScreenPoint:(CGPoint)point;

/**
 Called when a marker is tapped on. To handle multiple marker taps, see tapOnMarkers.
 @param meMarker MEMarker object that contains information about the marker that was tapped.
 @param mapView The MEMapView on which the tap occurred.
 @param mapName The name of the map.
 @param screenPoint The point on the screen on where the tap occurred.
 @param markerPoint The relative point within the marker where the tap occurred. Useful for markers that have button UI on them.
 */
- (void) tapOnMarker:(MEMarker*) meMarker
		   onMapView:(MEMapView*) mapView
			 mapName:(NSString*) mapName
	   atScreenPoint:(CGPoint)screenPoint
	   atMarkerPoint:(CGPoint) markerPoint;

/**
 Called when one or more markers is tapped on.
 */
- (void) tapOnMarkers:(NSArray*) meMarkerTaps
			onMapView:(MEMapView*) mapView;

/**
 Called when a dynamic marker is tapped on. To handle multiple marker taps, see tapOnDynamicMarkers.
 @param markerName The unique name of the marker within the dynamic marker layer.
 @param mapView The MEMapView on which the tap occurred.
 @param mapName The name of the map.
 @param screenPoint The point on the screen on where the tap occurred.
 @param markerPoint The relative point within the marker where the tap occurred. Useful for markers that have button UI on them.
 */
- (void) tapOnDynamicMarker:(NSString*) markerName
				  onMapView:(MEMapView*) mapView
					mapName:(NSString*) mapName
			  atScreenPoint:(CGPoint) screenPoint
			  atMarkerPoint:(CGPoint) markerPoint;

/**
 Called when one or more dynamic markers is tapped on.
 */
- (void) tapOnDynamicMarkers:(NSArray*) meMarkerTaps
				   onMapView:(MEMapView*) mapView;


@end

/**
 The MEDynamicMarkerMapDelegate protocol defines a set of methods that you can use to receive dynamic marker map related update messages. Implement this protocol when you add dynamic marker maps.
 @warning *Important:* This protocol is provided only for backward compatibility. Use MEMarkerMapDelegate instead.
 */
@protocol MEDynamicMarkerMapDelegate<MEMarkerMapDelegate>

@end

/**The MEVectorMapDelegate protocol defines a set of methods that you can use to receive vector map related update messages. Implement this protocol when you add dynamic vector maps and you need to receive vector map related notifications*/
@protocol MEVectorMapDelegate <NSObject>

@optional

/** Returns the number of pixels used as a buffer between vector line segments and hit test points.*/
- (double) lineSegmentHitTestPixelBufferDistance;

/** Returns the number of pixels used as a buffer between vector line vertices and hit test points.*/
- (double) vertexHitTestPixelBufferDistance;

/**Called when a hit (tap) is detected on a vector line segment.*/
- (void) lineSegmentHitDetected:(MEMapView*) mapView
						mapName:(NSString*) mapName
						shapeId:(NSString*) shapeId
					 coordinate:(CLLocationCoordinate2D) coordinate
			  segmentStartIndex:(int) segmentStartIndex
				segmentEndIndex:(int) segmentEndIndex;

/**Called when a hit is detected on a vector line vertex.*/
- (void) vertexHitDetected:(MEMapView*) mapView
				   mapName:(NSString*) mapName
				   shapeId:(NSString*) shapeId
				coordinate:(CLLocationCoordinate2D) coordinate
			   vertexIndex:(int) vertexIndex;

/**Called whan a hit (tap) is detected on a polygon in a vector map layer that has enabled polygon hit testing.
 @param mapView The MEMapView object where the hit originated.
 @param polygonHits An array of one or more MEVectorGeometryHit objects.*/
- (void) polygonHitDetected:(MEMapView*) mapView
					   hits:(NSArray*)polygonHits;

/**Called when a hit (tap) is detected on an extruded (3d) polygon in a vector map layer that has enabled polygon hit testing.
 @param mapView The MEMapView object where the hit originated.
 @param polygonHits An array of one or more MEVectorGeometryHit objects.
 @param screenPoint The poin on the screen which the hit occured.*/
- (void) polygonHit3dDetected:(MEMapView*) mapView
						 hits:(NSArray*)polygonHits
				  screenPoint:(CGPoint) screenPoint;

@end

/**
 Describes the methods a class must implement to provide camera constraint information.
 */
@protocol MECameraConstraintDelegate <NSObject>

@required
/**
 Called after the virtual camera is moved, but before rendering is done. Gives the delegate an opportunity to reposition the camera. If you move the camera during this funciton, return YES, otherwise, return NO.
 */
-(BOOL) constraintEnforced:(MECameraPlacement*) currentCameraPlacement
				 meMapView:(MEMapView*) meMapView
	terrainMinHeightMeters:(double) terrainMinHeightMeters
	terrainMaxHeightMeters:(double) terrainMaxHeightMeters;
@end

/**
 Describes the methods an object must implement to act as an end-point for loading a variety of map resources.
 */
@protocol METileLoader <NSObject>

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

@end

/**
 Describes the method a class must implement to receive information about resource loading in an MEProfileView.
 */
@protocol MEProfileViewDelegate <NSObject>
@optional
- (void) rasterLoadComplete:(uint) profileMetadata
					  image:(UIImage*) image
					 points:(NSArray*) points /// An array of MEProfileViewPointWithPercentage
				   finished:(bool) finished;
- (void) vectorLoadComplete:(uint) profileMetadata
				 vectorData:(NSArray*) vectorData /// An array of MEProfileViewExtrudedVectorData
				   finished:(bool) finished;
- (void) markerLoadComplete:(uint) profileMetadata
					markers:(NSArray*) markers /// An array of MEProfileViewMarkerResourceData
				   finished:(bool) finished;
@end


