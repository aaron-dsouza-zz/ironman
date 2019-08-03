//  Copyright (c) 2012 BA3, LLC. All rights reserved.
#pragma once

//SDK Imports
#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <CoreLocation/CoreLocation.h>

//BA3 Imports
#import "MEMapView.h"
#import "MEInfo.h"
#import "METileProvider.h"
#import "MEColorBar.h"
#import "METileInfo.h"
#import "MEProtocols.h"
#import "MEPolygonStyle.h"
#import "MEAnimatedVectorCircle.h"
#import "MEAnimatedVectorReticle.h"
#import "MEMapInfo.h"
#import "MEHitTesting.h"
#import "METerrainTile.h"
#import "MEModelVertex.h"

@class MEGeometryGroup;

/**
 Provides control and resource management for an MEMapView object. Create an instance of this object and associated MEMapView object.
 */
@interface MEMapViewController : GLKViewController <UIAlertViewDelegate, METileLoader>

/**---------------------------------------------------------------------------------------
 * @name Initialization
 *  ---------------------------------------------------------------------------------------
 */

/**Initialize an instance of MEMapViewController and the MEMapView it controls and add the MEMapView as a child to the specified parent UIView.
 @param parentView The UIView in which to add the MEMapView.
 */
-(id) initAsChildOfView:(UIView*) parentView;

/**Initialize an instance of MEMapViewController and 'attach' it to an existing MEMapView. This is for applications that require fine-grained control over view hierarchies and need to create the MEMapView object independently of MEMapViewController initialization.
 @param meMapView An existing MEMapView object you have already created.
 */

-(id) initWithMapView:(MEMapView*) mapView;

/** Initialize the mapping engine. This function allocates resources for thread management, data caching, and map loading. Should be called after the cooresponding MEMapView is loaded.
 
	//Initialize map view and controller and add to a parent view
	- (void) initializeMappingEngine {
 _meMapViewController = [[MEMapViewController alloc]init];
 _meMapViewController.view = [[MEMapView alloc]init];
 _meMapViewController.view.bounds = _view.bounds;
 [_view addSubview:_meMapViewController.view];
 [_meMapViewController initialize];
	}
 
 */
- (void) initialize;

/** Free resources related to rendering. Should be called before deallocating mapping engine views.*/
- (void) shutdown;

/** Returns the MEMapView this view controller manages. This is the same as the view property simply cast to MEMapView* and primarily provided as a convenience.*/
@property (atomic, readonly) MEMapView* meMapView;

/**---------------------------------------------------------------------------------------
 * @name Adding and Removing Layers
 *  ---------------------------------------------------------------------------------------
 */

/**
 Adds a map layer whose type and properties are specified by a MEMapInfo object.
 */
- (void) addMapUsingMapInfo:(MEMapInfo*) meMapInfo;

/**
 Instructs the mapping engine to reload a map.
 @param mapName The name of the map to be refreshed.
 */
- (void) refreshMap:(NSString*) mapName;

/**
 Instructs the mapping engine to reload the specified portion of a map.
 @param mapName The name of the map.
 @param lowerLeft The lower-left point of the region to refresh.
 @param upperRight The upper-right point of the region to refresh.
 */
- (void) refreshMapRegion:(NSString*) mapName
				lowerLeft:(CLLocationCoordinate2D) lowerLeft
			   upperRight:(CLLocationCoordinate2D) upperRight;

/** Add an Altus map package.*/
- (void) addPackagedMap:(NSString*) mapName
		packageFileName:(NSString*) packageFileName;

- (void) addTerrainHeightMap:(NSString*) mapName
			 packageFileName:(NSString*) packageFileName;

- (void) addTerrainMeshMap:(NSString*) mapName
			 heightMapName:(NSString*) heightMapName
				  meshSize:(int)meshSize;

/** Adds a raster map generated from raw double data */
- (void) addDataGridMap:(NSString*)mapName
			  withArray:(double*)array
			  withWidth:(int)width
			 withHeight:(int)height
			 withBounds:(CGRect)bounds
		   withColorBar:(METerrainColorBar*)colorBar;

- (void) addDataGridFrame:(NSString*)mapName
				withArray:(double*)array
				withWidth:(int)width
			   withHeight:(int)height
			   withBounds:(CGRect)bounds;

- (void) setDataGridFrame:(NSString*)mapName
				withFrame:(float)frame;

- (void) addWeatherGridMap:(NSString*)mapName
		  withTileProvider:(METileProvider*)tileProvider
		 withColorBarNames:(NSArray*)colorBarNames
		withTileBorderSize:(uint)tileBorderSize;

- (void) addWeatherGridMapBlendTarget:(NSString*)blendTargetMapName
							   forMap:(NSString*)mapName;

- (void) setWeatherGridMapBlendFactor:(float)blendFactor
							   forMap:(NSString*)mapName;

/**
 Sets the base map (zOrder of 0) layer to be a tiled map of the specified image.
 @param tiledImage A 256x256 image that will be tiled for the entire map.
 */
- (void) setBaseMapImage:(UIImage*) tiledImage;


/**
 Sets the base map layer to be a tiled map of the specified cached image.
 @param cachedImageName The name of the cached image.
 */
- (void) setBaseMapCachedImage:(NSString*) cachedImageName;


/**
 Adds an MBTiles raster map. Generally, these maps are produced with TileMill from MapBox. The map is added using an internal native tile provider path. Map bounds and maximum zoom level are read from the MBTiles database. Internally, the mapping engine will use a completely native tile provider to load tiles from the MBTiles database and no extra image memory copies are ever created. This is the fastest way possible to load an MBTiles map using the mapping engine.
 
 @param mapName Unique name of the map layer
 @param fileName Fully qualified path to the mbtiles filename
 @param defaultTileName The name of a cached image to use if waiting for tiles to load.
 @param imageDataType Inidicates whether the raster tiles are JPG or PNG
 @param compressTextures YES to compress tile textures to a 2 byte format
 @param zOrder Layer order for this map
 @param loadingStrategy Controls how tiles get loaded, kLowestDetailFirst will load tiles top down (less efficient but better user experience, kHighestDetailOnly will load only tiles at the current zoom level, faster)
 */
- (void) addMBTilesMap:(NSString*) mapName
			  fileName:(NSString*) fileName
	   defaultTileName:(NSString*) defaultTileName
		 imageDataType:(MEImageDataType) imageDataType
	   compessTextures:(BOOL) compressTextures
				zOrder:(uint) zOrder
	   loadingStrategy:(MEloadingStrategy) loadingStrategy;

/** Removes all maps currently displayed and optionally flushes the cache.*/
- (void) removeAllMaps:(BOOL) clearCache;

/** Remove a map layer from the current view and optionally clear the internal caches. If caches are not cleared, and you re-enable the same map at a future point, cached data 'may' be used to draw the map. If you choose to clear the cache, the engine will re-request all map data if you later re-add that map. Whether or not you want to flush the cache depends on your useage scenario. For example, if your application has downloaded an updated map, you would want to remove the existing one and clear its data from the cache. If the user is just toggling a map on and off, you would not want to clear it data from the cache the cache.
 @param mapPath The name of the map file (or layer name).
 @param clearCache A boolean value that indicates whether or not to flush the map's data from the cache.*/
- (void) removeMap:(NSString*) mapPath
		clearCache:(BOOL) clearCache;

/**Intended for virtual tile providers, when called, tells the mapping engine to re-request all tiles for the specified map. This could be use, for example, for displaying weather where the map changes over time.*/
- (void) invalidateMap:(NSString*) mapName;

/** Returns an array me MEMapInfo objects, each specifiying information about a map currently loaded. */
- (NSArray*) loadedMaps;

/** Returns whether or not the specified map is currently enabled.*/
- (BOOL) containsMap:(NSString*) mapName;

/** Set the priority of a map layer.
 @param mapName The name of the map.
 @param priority Value to set the priority to. Default is 1. The lower the value, the greater the priority. Priority 0 is treated as a special case.
 In the general case you will not need to use this API. By default, all maps default to priority 1 and there is no special treatment to any layer with regards to how resources get scheduled for loading. The engine will not exceed the maxTilesInFlight setting in order to rate limit resource loading and requests to tile providers. The default tiles in flight setting is 7.
 In some scenarios, you might desire to have a level of control over priority and tiles in flight. For example, if you have a custom map where resources are served by your own METileProvider derived class and if the production of tiles for the map is CPU intensive, you might decide to make that layer be scheduled after other layers by increasing its priority number. In that way, it will not block maps that might load faster. This can give the perceptions of faster loading to the user.
 If you have an advanced tile provider (see the TileFactory examples in our reference application) that is already capable of throttling the processing of requests and you want to insure that it is never 'starved' by maps that may take a long time to load, you can set the priority of the layer to 0. But you must be diligent in doing this as maps with priority 0 will always have all visible tiles requested and outstanding tile reqeusts can go beyond the maxTilesInFlight setting.
 */
- (void) setMapPriority:(NSString*) mapName
			   priority:(int) priority;

/** Get the current priority vof a map layer.*/
- (int) getMapPriority:(NSString*) mapName;

/** Set the distance to search up the tree for tiles. For example, imagine you have added a map package that only has up to level 6 tiles, but the user has zoomed into to level 18. If you want to over-zoom the level 6 tiles at the level 18 zoom level, you would set the tile search depth to at least 12.
 @param mapName The name of the map.
 @param depth The number of levels to search backwards up the tile tree for tiles. */
- (void) setMapTileSearchDepth:(NSString*) mapName
						 depth:(unsigned int)depth;

/**---------------------------------------------------------------------------------------
 * @name Streaming Content
 *  ---------------------------------------------------------------------------------------
 */

/** Adds a raster map layer comprised of downloadable spherical mercator raster tiles.
 
	//Example: Adding a MapBox street map.
	[self.meMapViewController addInternetMap:@"MapBox Streets"
 urlTemplate:@"http://{s}.tiles.mapbox.com/v3/my_mapbox_id/{z}/{x}/{y}.png"
 subDomains:@"a,b,c,d"
 maxLevel:20
 zOrder:2
 workerCount:3
 useNetworkCache:YES
 compressTextures:NO];
 
 @param mapName Unique name of the map layer
 @param urlTemplate A URL of the form http://{s}.your_domain_name/{z}/{x}/{y}.png where {s} is an optional sub-domain list.
 @param maxLevel The 'deepest' zoom level of the map.
 @param zOrder The rendering order of this layer.
 @param workerCount The number of parallel tile downloads.
 @param useNetworkCache If YES, use enable NSURL caching.
 @param diskCacheFile If set, the name of a file to cache raster tiles into. If the file does not exist, an Altus raster package file will be created and used to store any downloaded tiles.
 @param compresTextures If YES, convert textures to 16-bit formats. This can cause visual artifacts but does use significantly less memory.
 @warning Spherical mercator maps only cover from -80.0 to 80.0 latitude. If you have a tile set produced with AltusRaster, you can have global coverage by adding those tile sets with the other addInternetMap function.
 */
- (void) addInternetMap:(NSString*) mapName
			urlTemplate:(NSString*) urlTemplate
			 subDomains:(NSString*) subDomains
			   maxLevel:(unsigned int) maxLevel
				 zOrder:(unsigned int) zOrder
			workerCount:(unsigned int) workerCount
		useNetworkCache:(BOOL) useNetworkCache
		  diskCacheFile:(NSString*) diskCacheFile
	   compressTextures:(BOOL) compressTextures;

/** Adds a raster map layer comprised of downloadable spherical mercator raster tiles as well as polar map tiles in the proper projection (generally created with AltusRaster).
 
	//Example:
	[self.meMapViewController addInternetMap:@"Night Map"
 urlTemplateCenter:@"http://dev1.ba3.us/maps/raster/night/{z}/{x}/{y}.png"
 urlTemplateNorth:@"http://dev1.ba3.us/maps/raster/night/altn/{id}.png"
 urlTemplateSouth:@"http://dev1.ba3.us/maps/raster/night/alts/{id}.png"
 subDomains:@""
 maxLevel:8
 zOrder:4
 workerCount:3
 useNetworkCache:YES
 diskCacheFile:@"Night Map Cache.sqlite"
 compressTextures:NO];
 
 */
- (void) addInternetMap:(NSString*) mapName
	  urlTemplateCenter:(NSString*) urlTemplateCenter
	   urlTemplateNorth:(NSString*) urlTemplateNorth
	   urlTemplateSouth:(NSString*)urlTemplateSouth
			 subDomains:(NSString*) subDomains
			   maxLevel:(unsigned int) maxLevel
				 zOrder:(unsigned int) zOrder
			workerCount:(unsigned int) workerCount
		useNetworkCache:(BOOL) useNetworkCache
		  diskCacheFile:(NSString*) diskCacheFile
	   compressTextures:(BOOL) compressTextures;

/**---------------------------------------------------------------------------------------
 * @name Resource Management
 *  ---------------------------------------------------------------------------------------
 */

/**
 This function is provided for applications that desire to manage timing and updates themselves. If you are not managing your own CADisplayLink timer, there is no need to ever call this function. If you desire to use this function, you should first call stopDisplayLink, then create your own CADisplayLink timer, start it, and when it fires call this function passing the CADisplayLink timestamp value each time you want the mapping engine to update and draw.
 @param timestamp The timestamp value of the CADisplayLink timer you are managing.
 */
- (void) updateWithTimestamp:(CFTimeInterval) timestamp;

/**
 Enables or disables multithreaded resource loading. The default is YES.
 */
@property (nonatomic, getter=isMultithreaded) BOOL multiThreaded;

/**Indicates whether or not this instance of the view controller is initialized.*/
@property (nonatomic, readonly, assign) BOOL isInitialized;

/**
 If set to YES, prior to calling initialize, Altus will use its first-gen
 thread management system.
 */
@property (nonatomic) BOOL useLegacyResourceManager;

/** Returns true if the engine has been initialize, false otherwise. The view controller may still be in a paused state if the engine is running, but no frames will be presented.*/
@property (atomic, assign, readonly) BOOL isRunning;

/** The core in-memory cache size in bytes. Defaults to 60 MB on single-core devices, and 90 MB on dual-core devices. You should adjust this setting before calling the initialize function.*/
@property (assign) unsigned long coreCacheSize;

/** Controls the maximum number of tiles in flight per frame. Defaults to 3 on single-core devices, and 10 on dual-core devices. You should adjust this setting before calling the intialize function.*/
@property (assign) unsigned int maxTilesInFlight;

/** Controls the maximum number of tiles that will be loaded per frame that are supplied by asynchronous tile providers. This is especially helpful for animated tile providers that may be returning a large numbers of frames at once and do not with to cause a frame-rate hitch. The default is 1 tile per frame. This only applies to tiles that supply image data. Tile provider responses that indicate no loading is neccessary (not available or inivisible) are all handled every frame.*/
@property (assign) unsigned int maxAsyncTileLoadsPerFrame;

/** Controls the the number of levels up that parent tiles will be searched in virtual maps that are supplied tiles by tiler providers. The defaul is 5. Reducing this number can potentially save bandwidth in the case of internet-based tile providers, but at the cost of the user potentially not seeing map data if they zoom or pan quickly as tiles are downloading.*/
@property (assign) unsigned int maxVirtualMapParentSearchDepth;

/** Controls the target frames per second to render when current run loop is in UITrackingRunLoopMode. This occurs, for example, when a table view is being scrolled. The default is 10.*/
@property (assign) unsigned int uiTrackingRunLoopPreferredFramesPerSecond;

/** When set to YES, the mapping engine will attempt to reduce the framerate when possible.*/
@property (assign) BOOL greenMode;

/** The frame rate to render at when in green mode, resources are not loading, there are no animations, the camera is not moving, and the user is not interacting with the map. Defaults to 1 fps.*/
@property (assign) unsigned int greenModeIdleFramerate;

/** The frame rate to render at when an animation is occurring, resources are loading, or the camera is moving. Defaults to 12.*/
@property (assign) unsigned int greenModeCruiseFramerate;

/** The frame rate to render at when the user is interacting with the map. Defaults to 30 fps.*/
@property (assign) unsigned int greenModeFullThrottleFramerate;

/** Vertical field of view in degrees when in portrait mode.*/
@property (assign) float portraitFOV;

/** Vertical field of view in degrees when in landscape mode.*/
@property (assign) float landscapeFOV;

/**
 Used to notify the view controller that a user interaction (panning/zoomin) or code-based panning/zooming has occured. This is primarily for green-mode support so that gesture recognizers can notify the view controller to increase the frame rate in response to user touches. This function may be made private in a future release.
 */
- (void) greenModeFulleThrottle;

/** When set to YES, UIImages provided for markers are scaled by the map view content scale factor, and not by their UIImage scale. In other words, an @3x marker image will render so that it is the correct apparent size to the user on an iPhone 6 Plus running at the default scale of 2.6 (unzoomed), 2.8 (zoomed), or 3.0 (simulator).
 
 While this feature will allow the use of lower-resolution marker assets on high resolution displays this is not the intended purpose of the feature. You should scale your marker content so it renders well at the physical resolution of the display. Due to the iPhone 6 and iPhone 6 Plus having variable scaling, this setting defaults to YES.
 
 If the UIImage provided for a marker is a different scale than map view content scale factor and this setting is disabled, then marker locations, anchor points, hit-test sizes, and reported hit test coordinates could be incorrect. The mapping engine will issue a warning when you attempt to add a marker image with mis-matched scale and this feature is disabled. In the case where this feature is enabled and the image scale is mismatched, the engine will perform a magnification or minification of the incoming marker image to size it to the correct physical size. This can degrade marker image quality.*/
@property (assign) BOOL autoScaleMarkerImages;

/**---------------------------------------------------------------------------------------
 * @name Realism
 *  ---------------------------------------------------------------------------------------
 */

/** Toggles star field visibilty. Defaults to YES.*/
@property (nonatomic, assign) BOOL starsEnabled;

/** Toggles sun texture visibilty. Defaults to YES.*/
@property (nonatomic, assign) BOOL sunImageEnabled;

/**
 Controls the type of lighting.
 @see MELightingType.
 */
@property (assign) MELightingType lightingType;

/**
 Controls the transparency of the atmosphere.
 */
@property (nonatomic, assign) float atmosphereAlpha;

/**
 Set the virtual position of the sun. The position can be absolute or relative to the current camera position.
 @param location The point directly below the sun.
 @param locationType Absolute or relative position.
 */
- (void) setSunLocation:(CLLocationCoordinate2D) location
		andLocationType:(MELocationType) locationType;

/**Change the image used to represent the sun.
 @param image The new sun image to use.*/
- (void) setSunImage:(UIImage*)image;

/**
 Controls atmospheric lighting colors.
 @param red Amount of red (0 to 1)
 @param green Amount of green (0 to 1)
 @param blue Amount of blue (0 to 1)
 */
- (void) setLightWavelengths:(float)red
					   green:(float)green
						blue:(float)blue;

/**
 Controls atmospheric lighting parameters.
 */
- (void) setExposureAtmosphereRayleigh:(float)rayleigh
				 atmosphereMieExposure:(float)mie
						planetExposure:(float)planet;

/**---------------------------------------------------------------------------------------
 * @name Camera Control
 *  ---------------------------------------------------------------------------------------
 */

/**If you have set the METrackUp rendering mode, you can set the trackup-forward distance with this function.
 @param points The distance in screen points to move the camera forward from the pivot point where rotation occurs when heading is adjusted.
 @param animationDuration If animated, the animation interval in seconds, zero otherwise.
 */
- (void) setTrackUpForwardDistance:(double) points
				 animationDuration:(double) animationDuration;

/** Returns the current track up forward distance. */
- (double) getTrackUpForwardDistance;

/**If you have set the METrackUp rendering mode, you can set the trackup-forward distance with this function.
 @param points The distance in screen points to move the camera forward from the pivot point where rotation occurs when heading is adjusted.
 */
- (void) setTrackUpForwardDistance:(double) points;

/**---------------------------------------------------------------------------------------
 * @name Layer Appearance
 *  ---------------------------------------------------------------------------------------
 */

/** Set the alpha value of a map layer.
 @param mapName The name of the map file wihout the file extension.
 @param alpha Value to set alpha to. Range is 0 to 1.*/
- (void) setMapAlpha:(NSString*) mapName
			   alpha:(CGFloat) alpha;

/** Get the alpha value of a map layer.
 @param mapName The name of the map.*/
- (double) getMapAlpha:(NSString*) mapName;

/**Sets whether or not a loaded map is visible or not visible. This is not the same as setting the alpha to 0.0. When alpha is set to 0.0, the engine will still draw the map 'invisibly' going through all drawing logic. When the map is not visible, all loading logic for a map occurs, but the drawing logic is skipped. This can be used, for example, if you want to add a map, set it to invisible, wait for the engine to load it, then when your delegate is notified that loading is complete, to set visibilty to true to have the map instantly appear without the user seeing loading.*/
- (void) setMapIsVisible:(NSString*) mapName
			   isVisible:(BOOL) isVisible;

/**Returns whether or not a map is visible.*/
- (BOOL) getMapIsVisible:(NSString*) mapName;

/** Set the zorder value of a map layer.
 @param mapName The name of the map.
 @param zOrder Value to set the zorder to.*/
- (void) setMapZOrder:(NSString*) mapName
			   zOrder:(int) zOrder;

/** Get the current zorder value of a map layer.*/
- (int) getMapZOrder:(NSString*) mapName;

/** Sets per-map lighting parameters
 @param minLightScale Minimum intensity that a map texture scaled on the dark side of the planet
 @param nightAlphaMaskEnabled Enables an automatic alpha mask based on sun position that is fully opaque on the night side and transparent on the day side
 */
- (void) setMapLighting:(NSString*) mapName
		  minLightScale:(float)minLightScale
  nightAlphaMaskEnabled:(BOOL)nightAlphaMaskEnabled;


/**---------------------------------------------------------------------------------------
 * @name Terrain Coloring
 *  ---------------------------------------------------------------------------------------
 */

/**Updates the terrain color bar. When updated, the base map layer derived from terrain will be colored using the new color bar. 3D terrain will also use these colors. If not set, the mapping engine will use a default terrain color bar. You should set this value before beginning animation, otherwise cached tiles may not inherit the new colors.
 */
-(void) updateTerrainColorBar:(METerrainColorBar*) terrainColorBar;

/**Returns the current terrain color bar.*/
-(METerrainColorBar*) terrainColorBar;

/** Updates the terrain avoidance and warning system (TAWS) color bar. If not set, the mapping engine will use a default TAWS color bar. You can set the color bar at any time.
 */
-(void) updateTawsColorBar:(METerrainColorBar*)colorBar;

/**Returns the current terrain avoidance and warning system (TAWS) color bar.*/
-(METerrainColorBar*) tawsColorBar;

/** Updates the altitude that is used to draw the TAWS layer (if enabled).
 */
-(void) updateTawsAltitude:(double)altitude;


/**---------------------------------------------------------------------------------------
 * @name Marker Layers
 *  ---------------------------------------------------------------------------------------
 */

/**
 Add a dynamic marker to a dynamic marker map.
 @param mapName Unique name of the dynamic marker map layer.
 @param dynamicMarker Object that fully describes the dynamic marker. Either uiImage or cachedImageName MUST be set.
 */
- (void) addDynamicMarkerToMap:(NSString*) mapName dynamicMarker:(MEMarker*) dynamicMarker;
- (void) removeDynamicMarkerFromMap:(NSString*) mapName markerName:(NSString*) markerName;
- (void) updateDynamicMarkerImage:(NSString*) mapName markerName:(NSString*) markerName uiImage:(UIImage*) uiImage anchorPoint:(CGPoint) anchorPoint offset:(CGPoint) offset compressTexture:(BOOL) compressTexture;
- (void) updateDynamicMarkerImage:(NSString *) mapName markerName:(NSString *) markerName cachedImageName:(NSString*) cachedImageName anchorPoint:(CGPoint) anchorPoint offset:(CGPoint) offset;
- (void) updateDynamicMarkerLocation:(NSString*) mapName markerName:(NSString*) markerName location:(CLLocationCoordinate2D) location animationDuration:(double) animationDuration;

- (void) updateDynamicMarkerRotation:(NSString*) mapName
						  markerName:(NSString*) markerName
							rotation:(double) rotation
				   animationDuration:(double) animationDuration;

- (void) updateDynamicMarkerAltitude:(NSString*) mapName
						  markerName:(NSString*) markerName
							altitude:(double) altitude
				   animationDuration:(double) animationDuration;

- (void) setDynamicMarkerMapColorBar:(NSString*)mapName colorBar:(MEHeightColorBar*) colorBar;
- (void) hideDynamicMarker:(NSString*) mapName markerName:(NSString*) markerName;
- (void) showDynamicMarker:(NSString*) mapName markerName:(NSString*) markerName;



/** Sets a height color bar for the specified marker map. Once set you can call setMarkerMapColorBarEnabled to turn use of the color bar on and off.*/
- (void) setMarkerMapColorBar:(NSString*)mapName
					 colorBar:(MEHeightColorBar*) colorBar;

/** Sets where or not a color bar is applied when rendering markers.*/
- (void) setMarkerMapColorBarEnabled:(NSString*)mapName
							 enabled:(BOOL) enabled;

/**
 Returns an array of MEMarker objects for markers that are visible for a given marker map layer. This function should only be called form the main thread, not from a background thread.
 @param mapName Name of currently loaded marker layer.
 */
- (NSArray*) getVisibleMarkers:(NSString*) mapName;

/**---------------------------------------------------------------------------------------
 * @name Vector Layers
 *  ---------------------------------------------------------------------------------------
 */

/**Adds a polygon to a vector map.
 @param mapName The name of the vector map.
 @param points An array of NSValue objects that wrap CGPoints. The x,y values of the point represent longitude,latitude.
 @param style The style of the polygon.*/
- (void) addPolygonToVectorMap:(NSString*) mapName
						points:(NSMutableArray*)points
						 style:(MEPolygonStyle*)style;

/**Adds a polygon to a vector map with the specified style. If you have many polygons of the same style, it is more efficient (less draw calls) to use the other funciton which takes a featureId instead of a style.
 @param mapName The name of the vector map.
 @param polygonId The polygon's unique name that is returned during hit testing.
 @param points  An array of NSValue objects that wrap CGPoints. The x,y values of the point represent longitude,latitude.
 @param style The style to apply.*/
- (void) addPolygonToVectorMap:(NSString*) mapName
					 polygonId:(NSString*) polygonId
						points:(NSMutableArray*)points
						 style:(MEPolygonStyle*)style;

/**Adds a polygon to a vector map and applies a previously added style based featureId. If you have many polygons with the same style, this is the most efficient way to add them.
 @param mapName The name of the vector map.
 @param polygonId The polygon's unique name that is returned during hit testing.
 @param points An array of NSValue wrapped CGPoints where x is longitude, y is latitude that created a closed shape.
 @param featureId Identifier for the feature used to determine style.*/
- (void) addPolygonToVectorMap:(NSString*) mapName
					 polygonId:(NSString*) polygonId
						points:(NSMutableArray*)points
					 featureId:(unsigned int) featureId;

/**Updates a style previously set for a polygon in a vector map.
 @param mapName The name of the vector map.
 @param polygonId The polygon's unique name.
 @param style The style to apply.
 @param animationDuration The duration in seconds to animation to this style from the previous style*/
- (void) updatePolygonStyleInVectorMap:(NSString*) mapName
							 polygonId:(NSString*)polygonId
								 style:(MEPolygonStyle*)style
					 animationDuration:(CGFloat)animationDuration;

- (void) setStyleSetOnVectorMap:(NSString*) mapName
				  styleFileName:(NSString*) styleFileName
				   styleSetName:(NSString*) styleSetName;

/**Adds a style to a feature in a vector map.
 @param mapName The name of the vector map.
 @param style The style to apply.*/
- (void) addPolygonStyleToVectorMap:(NSString*) mapName
						  featureId:(unsigned int) featureID
							  style:(MEPolygonStyle*)style;

/**Adds a style to a feature in a vector map.
 @param mapName The name of the vector map.
 @param style The style to apply.*/
- (void) addLineStyleToVectorMap:(NSString*) mapName
						  styleName:(NSString*) styleName
							  style:(MELineStyle*)style;

/**Updates a style previously set for a feature in a vector map.
 @param mapName The name of the vector map.
 @param featureID The polygon feature of the map to apply the style to.
 @param style The style to apply.
 @param animationDuration The duration in seconds to animate to this style from the previous style*/
- (void) updatePolygonStyleInVectorMap:(NSString*) mapName
							 featureId:(unsigned int) featureID
								 style:(MEPolygonStyle*)style
					 animationDuration:(CGFloat)animationDuration;

/**Adds a scale-dependent style to a feature in a vector map.
 @param mapName The name of the vector map.
 @param featureID The polygon feature of the map to apply the style to.
 @param scale the target scale for the style
 @param style The style to apply.*/
- (void) addPolygonStyleToVectorMap:(NSString*) mapName
							  scale:(double) scale
						  featureId:(unsigned int) featureID
							  style:(MEPolygonStyle*)style;

/**Adds a line to a vector map.
 @param mapName The name of the vector map.
 @param points An array of NSValue objects that wrap CGPoints. The x,y values of the point represent longitude,latitude for each point in the line.
 @param style The style of the polygon.*/
- (void) addLineToVectorMap:(NSString*) mapName
					 points:(NSArray*)points
					  style:(MELineStyle*)style;

/**Adds a line to a vector map.
 @param mapName The name of the vector map.
 @param points An array of NSValue objects that wrap CGPoints. The x,y values of the point represent longitude,latitude for each point in the line.
 @param style The style of the polygon.*/
- (void) addLineToVectorMap:(NSString*) mapName
					 points:(NSArray*)points
				  styleName:(NSString*)styleName;

/**Adds a dynamic line to a dynamic vector map.
 @param mapName The name of the vector map.
 @param lineId The identifier of the line (relevant for hit testing).
 @param points An array of NSValue objects that wrap CGPoints. The x,y values of the point represent longitude,latitude for each point in the line.
 @param style The style of the line.*/
- (void) addDynamicLineToVectorMap:(NSString*) mapName
							lineId:(NSString*) lineId
							points:(NSArray*)points
							 style:(MELineStyle*)style;

/**Adds a dynamic line to a dynamic vector map.
 @param mapName The name of the vector map.
 @param lineId The identifier of the line (relevant for hit testing).
 @param startLocation The starting point of the line.
 @param endLocation The ending point of the line.
 @param style The style of the line.
 @param animationDuration If animated, the animation interval in seconds, zero otherwise.*/
- (void) addDynamicLineToVectorMap:(NSString*) mapName
							lineId:(NSString*) lineId
					 startLocation:(CLLocationCoordinate2D) startLocation
					   endLocation:(CLLocationCoordinate2D) endLocation
							 style:(MELineStyle*)style
				 animationDuration:(CGFloat) animationDuration;

/**Adds a dynamic 3d line to a dynamic vector map.
 @param mapName The name of the vector map.
 @param lineId The identifier of the line (relevant for hit testing).
 @param points An array of NSValue objects that encode MELocation3D.
 @param styleName The name of the style of the line.*/
- (void) addDynamicLine3dToVectorMap:(NSString*) mapName
							  lineId:(NSString*) lineId
							  points:(NSArray*)points
						   styleName:(NSString*)styleName;

/**Adds a dynamic 3d line to a dynamic vector map.
 @param mapName The name of the vector map.
 @param lineId The identifier of the line (relevant for hit testing).
 @param points An array of NSValue objects that encode MELocation3D.
 @param style The style of the line.*/
- (void) addDynamicLine3dToVectorMap:(NSString*) mapName
							  lineId:(NSString*) lineId
							  points:(NSArray*)points
							   style:(MELineStyle*)style;


/**Adds a dynamic 3d line skirt to a dynamic vector map.
 @param mapName The name of the vector map.
 @param shapeId The identifier of the shape (relevant for hit testing).
 @param points An array of NSValue objects that encode MELocation3D.
 @param style The style of the polygon.
 @param extrudeAltitude The skirt gets stretched vertical to this altitude*/
- (void) addDynamicLine3dSkirtToVectorMap:(NSString*) mapName
								  shapeId:(NSString*) shapeId
								   points:(NSArray*)points
									style:(MEPolygonStyle*)style
						  extrudeAltitude:(double)altitude;

- (void) addDynamicPolygon3dIntersectionToVectorMap:(NSString*) mapName
											shapeId:(NSString*) shapeId
									  polygonPoints:(NSArray*)polygonPoints
							  polygonBottomAltitude:(double)polygonBottomAltitude
								 polygonTopAltitude:(double)polygonTopAltitude
										 linePoints:(NSArray*)linePoints
							  lineBottomAltitude:(double)lineBottomAltitude
								 lineTopAltitude:(double)lineTopAltitude
											  style:(MEPolygonStyle*)style;

/**Adds a dynamic polygon to a dynamic vector map.
 @param mapName The name of the vector map.
 @param shapeId The identifier of the shape (relevant for hit testing).
 @param points An array of NSValue objects that wrap CGPoints. The x,y values of the point represent longitude,latitude for each point in the line.
 @param style The style of the polygon.*/
- (void) addDynamicPolygonToVectorMap:(NSString*) mapName
							  shapeId:(NSString*) shapeId
							   points:(NSArray*)points
								style:(MEPolygonStyle*)style;

/**Adds a dynamic 3d polygon to a dynamic vector map.
 @param mapName The name of the vector map.
 @param shapeId The identifier of the shape (relevant for hit testing).
 @param points An array of NSValue objects that wrap MELocation3D structs
 @param style The style of the polygon.*/
- (void) addDynamicPolygon3dToVectorMap:(NSString*) mapName
								shapeId:(NSString*) shapeId
							  points:(NSArray*)points
							   style:(MEPolygonStyle*)style;

/**Adds a dynamic 3d polygon extruded using the top and bottom altitude
 @param mapName The name of the vector map.
 @param shapeId The identifier of the shape (relevant for hit testing).
 @param points An array of NSValue objects that wrap CLLocationCoordinate2D structs
 @param style The style of the polygon.
 @param topAltitude The top of the extruded polygon
 @param bottomAltitude The bottom of the extruded polygon */
- (void) addDynamicPolygon3dToVectorMap:(NSString*) mapName
								shapeId:(NSString*) shapeId
							  points:(NSArray*)points
							   style:(MEPolygonStyle*)style
							topAltitude:(double)topAltitude
						 bottomAltitude:(double)bottomAltitude;


/**Adds an ESRI shape file to an in-memory vector map.
 @param mapName The name of the vector map.
 @param shapeFilePath Full path to the shape file.
 @param style The style of the line with which to render the shape.*/
-(void) addShapeFileToVectorMap:(NSString*) mapName
				  shapeFilePath:(NSString*) shapeFilePath
						  style:(MELineStyle*)style;


/**Clears all dynamic lines and polygons from a vector map.
 @param mapName The name of the vector map.*/
- (void) clearDynamicGeometryFromMap:(NSString*) mapName;

/**Used to exagerate the altitude of 3d lines and polygons
 @param altitudeScale scale factor with default of 1.0 */
- (void) setAltitudeScale:(double)altitudeScale withAnimationDuration:(double)duration;

/**Returns an object if there is vector geometry present at the screen point. Use this for detecting when the user has touched vector geometry. Returns nil if nothing is pressed. If the vectorDelegate is set, also calls hitdetection methods of the delegate.
 @param mapName Name of the vector map layer on which to search for hits.
 @param screenPoint Screen position in points to check.
 @param vertexHitBuffer Radius in points around vertices in which to detect hits.
 @param lineHitBuffer Distance in points from lines in which to detect hits.*/
- (MEVectorGeometryHit*) detectHitOnMap:(NSString*) mapName
						  atScreenPoint:(CGPoint) screenPoint
					withVertexHitBuffer:(CGFloat) vertexHitBuffer
					  withLineHitBuffer:(CGFloat) lineHitBuffer;

- (NSArray*) detectHitsAtPoint:(CGPoint)screenPoint;

/**Sets tesselation threshold for lines in nautical miles. */
- (void) setTesselationThresholdForMap:(NSString*) mapName
						 withThreshold:(CGFloat) threshold;

/**Force all lines to use 3d line rendering path. */
- (void) force3DLinesForMap:(NSString*)mapName
			   force3DLines:(BOOL)enabled;

/**---------------------------------------------------------------------------------------
 * @name Transparency Masks
 *  ---------------------------------------------------------------------------------------
 */

/** Allow one map to clip another map. Anywhere the clip map would draw is not be drawn by the target map.*/
- (void) addClipMapToMap:(NSString*)mapName
			 clipMapName:(NSString*)clipMapName;

/**---------------------------------------------------------------------------------------
 * @name Custom Content
 *  ---------------------------------------------------------------------------------------
 */

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


/**---------------------------------------------------------------------------------------
 * @name Animated Vector Markers
 *  ---------------------------------------------------------------------------------------
 */

/**Adds an animated vector circle. This can be used for pulsating location beacons.*/
- (void) addAnimatedVectorCircle:(MEAnimatedVectorCircle*) animatedVectorCircle;

/**Update the position of an animated vector circle.*/
- (void) updateAnimatedVectorCircleLocation:(NSString*) name
								newLocation:(CLLocationCoordinate2D)newLocation
						  animationDuration:(CGFloat) animationDuration;

/**Removes an animated vector circle.*/
- (void) removeAnimatedVectorCircle:(NSString*) name;

/**Adds an animated reticle to mark locations.*/
- (void) addAnimatedVectorReticle:(MEAnimatedVectorReticle*) animatedVectorReticle;

/**Update the position of an animated reticle.*/
- (void) updateAnimatedVectorReticleLocation:(NSString*) name
								 newLocation:(CLLocationCoordinate2D)newLocation
						   animationDuration:(CGFloat) animationDuration;

/**Removes an animated reticle.*/
- (void) removeAnimatedVectorReticle:(NSString*) name;


/**---------------------------------------------------------------------------------------
 * @name Image Caching
 *  ---------------------------------------------------------------------------------------
 */

/** Adds an image that will stay cached until it is removed using removeCachedImage. Cached images are identified by their name and may be specified as the default tile for certain maps types or returned by tile providers that have no specific tile to return for a given tile request. Generally this should be a fully opaque 256x256 or 512x512 pixel image.
 @param uiImage A UIImage containing the image data.
 @param imageName The unique name of the image.
 @param compressTexture Whether or not the image should be compressed to RGB565 format.*/
- (void) addCachedImage:(UIImage*) uiImage
			   withName:(NSString*) imageName
		compressTexture:(BOOL) compressTexture;

/** Checks if a cached image exists for the given name
 @param imageName The name of the image to check. */
- (bool) hasCachedImagedWithName:(NSString*) imageName;

/** Adds a marker image that will stay cached until it is removed using removeCachedImage. The intent is for the image to be used as a marker image. Cached marker images are identified by their name and may be specified as the image to be used for a marker when a marker is added. To changed the cached image, call the function again with the same image name, but a different image.*/
- (void) addCachedMarkerImage:(UIImage*) uiImage
					 withName:(NSString*) imageName
			  compressTexture:(BOOL) compressTexture
nearestNeighborTextureSampling:(BOOL) nearestNeighborTextureSampling;

- (void) addColorBar:(MEHeightColorBar*)colorBar withName:(NSString*)imageName;


/** Removes a cached image or cached marker image that was previously added with the addCachedImage or addCachedMarkerImage function.*/
-(void) removeCachedImage:(NSString*) tileName;

-(void) addProfileImage:(NSString*)path onQueue:(dispatch_queue_t)queue onComplete:(void (^)(NSString*))onComplete;
/**---------------------------------------------------------------------------------------
 * @name User Interface Support
 *  ---------------------------------------------------------------------------------------
 */

/** Returns an angle relative to the verticle edge of the view that represents the rotation you would apply to a screen-aligned object so that it points to the given heading. You would use this function, for example, if you wanted to display an 2D arrow that points in at heading. If you desire for an object to do this and always be up to date and smoothly animated, you should use a marker layer with a marker whose rotation type is kMarkerRotationTrueNorthAligned, then the mapping engine will manage the rotation of the object.*/
-(CGFloat) screenRotationForLocation:(CLLocationCoordinate2D) location
						 withHeading:(CGFloat) heading;

/** Returns an angle relative to the verticle edge of the view that represents the rotation you would apply to a screen-aligned object so that it points to the given heading releative to the current geographic point at the center of the view. You would use this function, for example, if you wanted to display an 2D arrow that points in at heading. If you desire for an object to do this and always be up to date and smoothly animated, you should use a marker layer with a marker whose rotation type is kMarkerRotationTrueNorthAligned, then the mapping engine will manage the rotation of the object.*/
-(CGFloat) screenRotationForMapCenterWithHeading:(double) heading;

/**---------------------------------------------------------------------------------------
 * @name License and Version
 *  ---------------------------------------------------------------------------------------
 */

/**
 Activate your license and disable the BA3 watermark.
 NOTE: Altus Developer Network members should use alternative setLicenseKey function which takes 2 parameters. Should be called after initialize has been called:
 
	//Activate license
	[_meMapViewController initialize];
	[_meMapViewController setLicenseKey:@"YOURLICE-NSEK-EYGO-ESIN-THISFUNCTION"];
 
 @param licenseKey Your license key.
 */
- (void) setLicenseKey:(NSString*) licenseKey;

/**
 Activate your license and disable the BA3 watermark for members of the Altus Developer Network. Should be called after initialize has been called:
 
	//Activate license
	[_meMapViewController initialize];
	[_meMapViewController	setLicenseKey:@"YOURLICE-NSEK-EYGO-ESIN-THISFUNCTION"
					licensedDeveloper:@"your_email_address@your.domain"];
 
 @param licenseKey Your license key.
 @param licensedDeveloper Your identifier on the Altus Developer Network.
 */
- (void) setLicenseKey:(NSString*) licenseKey
	 licensedDeveloper:(NSString*) licensedDeveloper;

/**
 Returns the version tag for this build of Altus.
 */
+ (NSString*) getVersionTag;

/**
 Returns the internal version hash for this build of Altus. You will be asked to provide this value for certain types of support requests.
 */
+ (NSString*) getVersionHash;

/**---------------------------------------------------------------------------------------
 * @name Analysis
 *  ---------------------------------------------------------------------------------------
 */

/** Contains information about the current state of the mapping engine.*/
@property (atomic, retain) MEInfo* meInfo;

/** When set to YES, tells the mapping engine to display verbose messages about state changes and activity.*/
@property (atomic, assign) BOOL verboseMessagesEnabled;

/** Semi-internal display list testing config. Should not be used except for testing display list against super slow but accurate method. */
- (void) setDisplayListMode:(int) mode;

- (void) setDisplayListCullingMode:(int) mode;

/** Very expensive but useful for stress testing or logging tile counts for different camera settings. */
- (int) computeTileCount;

- (void) setBiasSplitSize:(double) size;  // this might be used
- (void) setBiasCombineSize:(double) size;  // this might be used

/** Used to get larger and smaller tiles based on the tile slope to the screen. The default value is 1.4. Increase this value to reduce the number of tiles, or decrease this value to increase the number of tiles.*/
- (void) setTileDistanceScale:(double) scale;

/**
 @name 3D Models
 */

/**
 Adds a custom 3D model to a DynamicModel3D layer using raw vertices to specify points and indices into those vertices to specify points that make up each face (triangle) of the mesh for the model.
 @param mapName The name of the model map to add the model to.
 @param modelName The name of the model.
 @param vertices Pointer an an array of MEModelVertex structs (vertex buffer).
 @param numVertices The number of MEModelVertex structs in vertices.
 @param indices Pointer to an array of uint16_t integers that serve as indexes for triangles (index buffer).
 @param numIndeces The number of indices.
 @param texture A UIImage that will serve as a texture for the model.
 @param numInstances The number of instances of this model to create.
 @return On success, returns an integer index for the model. The index refers to the model's index as seen by the view instance. Otherwise, returns less than zero.
 */
- (int) addModelToMap:(NSString*)mapName
			modelName:(NSString*)modelName
			 vertices:(MEModelVertex*)vertices
		  numVertices:(uint)numVertices
			  indices:(uint16_t*)indices
		   numIndices:(uint)numIndices
		 textureImage:(UIImage*)textureImage
		 numInstances:(uint)numInstances;

/**
 Adds a 3D model to a DynamicModel3D layer using a JSON file that contains the model's mesh data. Model files can be generated using the AltusModel program (a component of AltusServer) to convert a variety of 3D model formats to JSON that can be loaded. Or you can create your own model JSON files. Refer to the AltusDemo program for some examples (i.e. Hexagon.json).
 @param mapName The name of the model map to add the model to.
 @param modelName The name of the model.
 @param modelFileName The name of the JSON file generated by AltusModel
 @param texture A UIImage that will serve as a texture for the model.
 @param numInstances The number of instances of this model to create.
 @return On success, returns an integer index for the model. The index refers to the model's index as seen by the view instance. Otherwise, returns less than zero.
 */
- (int) addModelToMap:(NSString*)mapName
			modelName:(NSString*)modelName
		modelFileName:(NSString*)modelFileName
		 textureImage:(UIImage*)textureImage
		 numInstances:(uint)numInstances;

/**
 Adds a 3D model to a DynamicModel3D layer using a string in JSON format. JSON model data can be generated using the AltusModel program (a component of AltusServer) to convert a variety of 3D model formats to JSON that can be loaded. Or you can create your own model JSON data. Refer to the AltusDemo program for some examples (i.e. Hexagon.json).
 @param mapName The name of the model map to add the model to.
 @param modelName The name of the model.
 @param modeJsonData A string that represents JSON data as generated by AltusModel
 @param texture A UIImage that will serve as a texture for the model.
 @param numInstances The number of instances of this model to create.
 @return On success, returns an integer index for the model. The index refers to the model's index as seen by the view instance. Otherwise, returns less than zero.
 */
- (int) addModelToMap:(NSString*)mapName
			modelName:(NSString*)modelName
		modelJsonData:(NSString*)modelJsonData
		 textureImage:(UIImage*)textureImage
		 numInstances:(uint)numInstances;

/**
 Adds a 3D cone to a DynamicModel3D layer.
 @param mapName The name of the model map to add the model to.
 @param modelName The name of the model.
 @param numSegments The number of segments that form the circle at the base of the cone.
 @param texture A UIImage that will serve as a texture for the model.
 @param numInstances The number of instances of this model to create.
 @return On success, returns an integer index for the model. The index refers to the model's index as seen by the view instance. Otherwise, returns less than zero.
 */
- (int) addConeModelToMap:(NSString*)mapName
				modelName:(NSString*)modelName
			  numSegments:(uint) numSegments
			 textureImage:(UIImage*)textureImage
			 numInstances:(uint)numInstances;

/**
 Adds a 3D cube to a DynamicModel3D layer.
 @param mapName The name of the model map to add the model to.
 @param modelName The name of the model.
 @param texture A UIImage that will serve as a texture for the model.
 @param numInstances The number of instances of this model to create.
 @return On success, returns an integer index for the model. The index refers to the model's index as seen by the view instance. Otherwise, returns less than zero.
 */
- (int) addCubeModelToMap:(NSString*)mapName
				modelName:(NSString*)modelName
			 textureImage:(UIImage*)textureImage
			 numInstances:(uint)numInstances;


/**
 Adds a 3D cylinder to a DynamicModel3D layer.
 @param mapName The name of the model map to add the model to.
 @param modelName The name of the model.
 @param numSegments The number of segments that form the circle at each end of the cylinder.
 @param texture A UIImage that will serve as a texture for the model.
 @param numInstances The number of instances of this model to create.
 @return On success, returns an integer index for the model. The index refers to the model's index as seen by the view instance. Otherwise, returns less than zero.
 */
- (int) addCylinderModelToMap:(NSString*)mapName
					modelName:(NSString*)modelName
				  numSegments:(uint) numSegments
				 textureImage:(UIImage*)textureImage
				 numInstances:(uint)numInstances;

/**
 Adds a 3D sphere to a DynamicModel3D layer.
 @param mapName The name of the model map to add the model to.
 @param modelName The name of the model.
 @param numSegments The number of segments used to tesselate the sphere.
 @param texture A UIImage that will serve as a texture for the model.
 @param numInstances The number of instances of this model to create.
 @return On success, returns an integer index for the model. The index refers to the model's index as seen by the view instance. Otherwise, returns less than zero.
 */
- (int) addSphereModelToMap:(NSString*)mapName
				  modelName:(NSString*)modelName
				numSegments:(uint) numSegments
			   textureImage:(UIImage*)textureImage
			   numInstances:(uint)numInstances;

/**
 Sets the placement and scale of a 3D model instance.
 @param modelIndex The index of the model returned when you added the model.
 @param instanceIndex The intance of the model you wish to transform.
 @param location The 2D geographic coordinate where the model should be placed.
 @param altitude The altitude in meters the model should be above sea level.
 @param scale How small or large to scale the model in muliples of the Earth's radius.
 @param yaw The horizontal rotation of the model with respect to the horizon.
 @param pitch The pitch of the model.
 @param roll The roll of the model.
 @param animationDuration Over how many seconds to animation the transformation.
 */
- (void) setModelTransform:(int)modelIndex
			 instanceIndex:(int)instanceIndex
				  location:(CLLocationCoordinate2D)location
				  altitude:(double)altitude
					 scale:(double)scale
					   yaw:(double)yaw
					 pitch:(double)pitch
					  roll:(double)roll
		 animationDuration:(double)animationDuration;

/**Updates the model texture*/
- (void) updateModelImage:(int)modelIndex
					image:(UIImage*)image;

/**---------------------------------------------------------------------------------------
 * @name Specific to iOS
 *  ---------------------------------------------------------------------------------------
 */

/** Forces linker to link this file via NIB-only interfaces.*/
+ (void) forceLink;

/**The EAGLContext in use by this view controller and its view.*/
@property (strong, nonatomic) EAGLContext *context;

/**Tells the engine when the application receives a memory warning from the system.*/
- (void) applicationDidReceiveMemoryWarning:(UIApplication *)application;

/**Forward this call from your AppDelegate so the mapping engine can manage internal state.*/
- (void)applicationWillResignActive:(UIApplication *)application;

/**Forward this call from your AppDelegate so the mapping engine can manage internal state.*/
- (void)applicationDidEnterBackground:(UIApplication *)application;

/**Forward this call from your AppDelegate so the mapping engine can manage internal state.*/
- (void)applicationWillEnterForeground:(UIApplication *)application;

/**Forward this call from your AppDelegate so the mapping engine can manage internal state.*/
- (void)applicationDidBecomeActive:(UIApplication *)application;

/**Forward this call from your AppDelegate so the mapping engine can manage internal state.*/
- (void)applicationWillTerminate:(UIApplication *)application;

/**
 Stops and invalidates the CADisplayLink timer that is used to drive updates. Provided for applications that desire to mananage timing and updates themselves. When stopped, the mapping engine will no longer update / draw and your application will be responsible for calling the updateWithTimestamp function in order to ensure updates occur.
 */
- (void) stopDisplayLink;

/**
 Creates and starts a CADisplayLink  timer that is used to drive updates. This function is provided for applications that desire to manage timing and updates themsevles. When started, you should discontinue calling updateWithTimestamp.
 */
- (void) startDisplayLink;

@end
