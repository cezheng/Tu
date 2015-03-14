#import <Foundation/Foundation.h>

typedef struct ISize {
    int w, h;
} ISize;

@interface ImagePathCache : NSObject
@property (atomic, strong, readonly) NSCache* pathCache;
@property (atomic, strong, readonly) NSCache* cropCache;
+ (id) sharedCache;
+ (NSString*) cropCacheKeyByPath:(NSString*)path
                            area:(CGRect)area;

- (id)imageWithPath:(NSString *)path;

- (id) imageWithPath:(NSString *)path
                area:(CGRect)area;

- (id)imageWithPath:(NSString *)path
           cropInfo:(NSDictionary*)info;

- (id) imageWithPath:(NSString *)path
         scaleToSize:(ISize)size;

- (id)imageWithPath:(NSString *)path
               area:(CGRect)area
        scaleToSize:(ISize)size;

- (id)imageWithPath:(NSString *)path
           cropInfo:(NSDictionary*)info
        scaleToSize:(ISize)size;

- (id)imageWithName:(NSString *)name
              scaleToSize:(ISize)size;
@end
