import json
import re

modelKeys = ['className', 'cppHeader', 'includes', 'baseClasses', 'cppConstructors',
             'instanceMethods', 'synthesizeProperties', 'staticMethods',
             'objcHeader', 'objcPrefix', 'objcSuffix', 'objcInit',
             'synthesizeProperties', 'wrapIn'
];

primitiveTypes = []

cppToObjcTypeCast = {
    'std::string' : 'NSString*'
}

cppToObjcTypeReplace = {
    'bool' : 'BOOL'
}

class XClassModel:
    def __init__(self, jsonObject, tableLen = 4):
        self.tabLen = tableLen
        for key in modelKeys:
            if key in jsonObject:
                exec('self.%s = jsonObject["%s"]' % (key, key))
            else:
                exec('self.%s = None' % (key))
    def toObjcType(self, cppType):
        if cppType in cppToObjcTypeCast:
            return cppToObjcTypeCast[cppType]
        elif cppType in cppToObjcTypeReplace:
            return cppToObjcTypeReplace[cppType]
        else:
            return cppType
    def genObjcSetterImplemenation(self, cppType, property):
        objcType = self.toObjcType(cppType)
        if cppType == 'std::string':
            return """
- (void) %(setter)s: (NSString*)%(argument)s {
    __cppObject__->%(setter)s([%(argument)s UTF8String]);
}
""" % {'setter' : self.genSetterName(cppType, property), 'argument' : property}
        else:
            return """
- (void) %(setter)s: (objcType)%(argument)s {
    __cppObject__->%(setter)s(%(argument)s);
}
""" % {'setter' : self.genSetterName(cppType, property), 'argument' : property, 'objcType' : objcType}


    def genObjcGetterImplemenation(self, cppType, property):
        objcType = self.toObjcType(cppType)
        if cppType == 'std::string':
            return """
- (NSString*) %(property)s {
    return @(__cppObject__->%(cppGetter)s().c_str());
}
""" % {'property' : property, 'cppGetter' : self.genCppGetterName(cppType, property)}
        else:
            return """
- (%(objcType)s) %(property)s {
    return __cppObject__->%(cppGetter)s();
}
""" % {'objcType': objcType, 'property': property, 'cppGetter': self.genCppGetterName(cppType, property)}

    def genCppGetterName(self, cppType, property):
        if type != 'bool':
            getterPrefix = 'get'
        else:
            getterPrefix = 'is'
        suffix = property[0].upper() + property[1:]
        return getterPrefix + suffix

    def genSetterName(self, cppType, property):
        if type != 'bool':
            setterPrefix = 'set'
        else:
            setterPrefix = 'setIs'
        suffix = property[0].upper() + property[1:]
        return setterPrefix + suffix

    def genObjcFile(self):
        tab = ' ' * self.tabLen
        objcClassName = self.objcPrefix + self.className + self.objcSuffix
        fileContent = """
#import "%(objcHeader)s"
#imclude "%(cppHeader)s"

@implementation %(objcClassName)s () {
    %(cppClassName)s* __cppObject__;
}
@end

- (id)initWithCppObject:(void*)obj {
    self = [super init];
    if (self) {
        __cppObject__ = (ChatMessage*)obj;
        if (!__cppObject__) self = nil;
    }
    return self;
}

- (void)dealloc {
    delete __cppObject__;
    //[super dealloc]; // uncomment if not using ARC
}
""" % {'cppClassName': self.className,
       'objcClassName': objcClassName,
       'cppHeader': self.cppHeader,
       'objcHeader': self.objcHeader}
        for propertyAttr in self.synthesizeProperties:
            property = propertyAttr['name']
            cppType = propertyAttr['cpp_type']
            fileContent += self.genObjcGetterImplemenation(cppType, property)
            if 'options' in propertyAttr and 'readonly' not in propertyAttr['options']:
                fileContent += self.genObjcSetterImplemenation(cppType, property)
        fileContent += '\n@end\n'
        return fileContent

    def genObjcHeader(self):
        ifndefMacro = '__%s__' % (re.sub(r'[\./\/]', r'_', self.objcHeader))
        objcClass = self.objcPrefix + self.className + self.objcSuffix
        properties = []
        for propertyAttr in self.synthesizeProperties:
            property = propertyAttr['name']
            cppType = propertyAttr['cpp_type']
            if 'options' in propertyAttr and propertyAttr['options']:
                optionsText = '(%s) ' % (', '.join(propertyAttr['options']))
            else:
                optionsText = ''
            properties.append('@property %(options)s%(objcType)s %(property)s;' % {'options' : optionsText, 'objcType' : self.toObjcType(cppType), 'property' : property})
        fileContent = """
#ifndef %(ifndefMacro)s
#define %(ifndefMacro)s

@interface %(objcClass)s : NSObject
@property (readonly, nonatomic) void* cppObject;
%(properties)s
- (id)initWithCppObject:(void*)obj;
%(initMethods)s
%(instanceMethods)s
%(classMethods)s
@end

#endif //  %(ifndefMacro)s
""" % {'objcClass' : objcClass,
       'ifndefMacro' : ifndefMacro,
       'properties' : '\n'.join(properties),
       'initMethods' : '',
       'instanceMethods' : '',
       'classMethods' : ''
        }
        return fileContent

    def genCppFile(self):
        tab = ' ' * self.tabLen
        fileContent = '#include "%s"\n\n' % (self.cppHeader)
        for constructorKey in self.cppConstructors:
            constructorAttr = self.cppConstructors[constructorKey]
            argList = []
            initList = []
            if 'auto_gen_unspecified' in constructorAttr and constructorAttr['auto_gen_unspecified']:
                for propertyAttr in self.synthesizeProperties:
                    property = propertyAttr['name']
                    if 'auto_gen_exclude' in constructorAttr and property in constructorAttr['auto_gen_exclude']:
                        continue
                    type = propertyAttr['cpp_type']
                    if "options" in propertyAttr and 'assign' in propertyAttr["options"]:
                        argumentType = type
                    else:
                        argumentType = 'const %s &' % (type)
                    argList.append('%s %s' % (argumentType, property))
                    initList.append("_%s(%s)" % (property, property))
            if 'arg_list' in constructorAttr:
                argList += [arg[0] for arg in constructorAttr['arg_list']]
            argListText = ', '.join(argList)
            if 'init_list' in constructorAttr:
                initList += ["_%s(%s)" % (prop[0], prop[1]) for prop in constructorAttr['init_list']]

            if initList:
                initListText = ' : ' + ', '.join(initList)
            else:
                initListText = ''
            constructorText = '%s::%s(%s)%s {\n}\n\n' % (self.className, self.className, argListText, initListText)
            fileContent += constructorText
        for access in self.instanceMethods:
            methods = self.instanceMethods[access]
            for method in methods:
                if 'arg_list' in methods[method]:
                    argListText = ', '.join([arg for arg in methods[method]['arg_list']])
                else:
                    argListText = ''
                fileContent += '%s %s::%s(%s) {\n}\n\n' % (methods[method]['type'], self.className, method, argListText)
        for propertyAttr in self.synthesizeProperties:
            property = propertyAttr['name']
            type = propertyAttr['cpp_type'];
            if type != 'bool':
                getterPrefix = 'get'
                setterPrefix = 'set'
            else:
                getterPrefix = 'is'
                setterPrefix = 'setIs'
            suffix = property[0].upper() + property[1:]
            fileContent += '%s %s::%s%s(){\n%sreturn _%s;\n}\n\n' % (type, self.className, getterPrefix, suffix, tab, property)
            if "options" in propertyAttr and not "readonly" in propertyAttr["options"]:
                if "assign" in propertyAttr["options"]:
                    argumentType = type
                else:
                    argumentType = 'const %s &' % (type)
                fileContent += 'void %s::%s%s(%s %s){\n%s_%s = %s;\n}\n\n' % (setterPrefix, self.className, suffix, argumentType, property, tab, property, property)
        return fileContent

    def genCppHeader(self):
        ifndefMacro = '__%s__' % (re.sub(r'[\./\/]', r'_', self.cppHeader))
        includeText = '\n'.join(['#include %s' % (file) for file in self.includes])
        baseClasses = [['%s %s' % (access, className) for className in self.baseClasses[access]] for access in ['public', 'protected', 'private'] if access in self.baseClasses]
        classContent = {'public': [], 'protected': [], 'private': []}
        tab = ' ' * self.tabLen
        for constructorKey in self.cppConstructors:
            constructorAttr = self.cppConstructors[constructorKey]
            if 'access' in constructorAttr:
                access = constructorAttr['access']
            else:
                access = 'public'
            argList = []
            if 'auto_gen_unspecified' in constructorAttr and constructorAttr['auto_gen_unspecified']:
                for propertyAttr in self.synthesizeProperties:
                    property = propertyAttr['name']
                    if 'auto_gen_exclude' in constructorAttr and property in constructorAttr['auto_gen_exclude']:
                        continue
                    type = propertyAttr['cpp_type']
                    if "options" in propertyAttr and 'assign' in propertyAttr["options"]:
                        argumentType = type
                    else:
                        argumentType = 'const %s &' % (type)
                    argList.append('%s %s' % (argumentType, property))
            if 'arg_list' in constructorAttr:
                argList = [' = '.join(arg) for arg in constructorAttr['arg_list']]
            constructorText = '%s%s(%s);' % (tab, self.className, ', '.join(argList))
            classContent[access].append(constructorText)
        for access in self.instanceMethods:
            methods = self.instanceMethods[access]
            for method in methods:
                if 'arg_list' in methods[method]:
                    argList = [arg for arg in methods[method]['arg_list']]
                else:
                    argList = []
                classContent[access].append('%s%s %s(%s);' % (tab, methods[method]['type'], method, ', '.join(argList)))
        for propertyAttr in self.synthesizeProperties:
            property = propertyAttr['name']
            if 'access' in propertyAttr:
                access = propertyAttr['access']
            else:
                access = 'private'
            type = propertyAttr['cpp_type'];
            classContent[access].append('%s%s _%s;' % (tab, type, property))
            if type != 'bool':
                getterPrefix = 'get'
                setterPrefix = 'set'
            else:
                getterPrefix = 'is'
                setterPrefix = 'setIs'
            suffix = property[0].upper() + property[1:]
            classContent['public'].append('%s%s %s%s();' % (tab, type, getterPrefix, suffix))
            if "options" in propertyAttr and not "readonly" in propertyAttr["options"]:
                if "assign" in propertyAttr["options"]:
                    argumentType = type
                else:
                    argumentType = 'const %s &' % (type)
                classContent['public'].append('%svoid %s%s(%s %s);' % (tab, setterPrefix, suffix, argumentType, property))


        classDeclaration = """
#ifndef %(ifndefMacro)s
#define %(ifndefMacro)s

""" % {'ifndefMacro' : ifndefMacro}
        classDeclaration += '%s\n\n' % (includeText)
        if self.wrapIn:
            classDeclaration += '%s\n' % (self.wrapIn[0])
        if baseClasses:
            classDeclaration += 'class %s : %s {\n' % (self.className, ' '.join(baseClasses))
        else:
            classDeclaration += 'class %s {\n' % (self.className)
        for access in ['public', 'protected', 'private']:
            if classContent[access]:
                classDeclaration += '%s:\n%s\n' % (access, '\n'.join(classContent[access]))
        classDeclaration += '};\n\n'
        if self.wrapIn:
            classDeclaration += '%s\n' % (self.wrapIn[1])
        classDeclaration += '#endif // %s' % (ifndefMacro)
        return classDeclaration


with open("test2.json", 'r') as f:
    s = f.read()
    jsonObj = json.loads(s)
    model = XClassModel(jsonObj)
    print model.genCppHeader()
    print model.genCppFile()
    print model.genObjcFile()
    print model.genObjcHeader()