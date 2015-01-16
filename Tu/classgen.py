#!/usr/bin/env python
import json
import re
import os

modelKeys = ['className', 'cppHeader', 'includes', 'baseClasses', 'cppConstructors',
             'instanceMethods', 'synthesizeProperties', 'staticMethods',
             'objcHeader', 'objcPrefix', 'objcSuffix', 'objcInit',
             'synthesizeProperties', 'wrapIn', 'cppNameSpace'
];

primitiveTypes = []

cppToObjcTypeCast = {
    'std::string' : 'NSString*'
}

cppToObjcTypeReplace = {
    'bool' : 'BOOL'
}

predefinedMethods = {'encodeJson' : '    virtual void encodeJson();\n    virtual void encodeJson(const rapidjson::Value & jsonObject);',
                     'decodeJson' : '    virtual void decodeJson();\n    virtual void decodeJson(const rapidjson::Value & jsonObject);'}

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
        if cppType != 'bool':
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

    def genObjcFile(self, force = False):
        tab = ' ' * self.tabLen
        objcClassName = self.objcPrefix + self.className + self.objcSuffix
        fileContent = \
"""#import "%(objcHeader)s"
#include "%(cppHeader)s"

@interface %(objcClassName)s () {
    %(cppNameSpace)s::%(cppClassName)s* __cppObject__;
}
@end

@implementation %(objcClassName)s

- (id)initWithCppObject:(void*)obj {
    self = [super init];
    if (self) {
        __cppObject__ = (%(cppNameSpace)s::%(cppClassName)s*)obj;
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
       'objcHeader': self.objcHeader,
       'cppNameSpace' : self.cppNameSpace}
        for propertyAttr in self.synthesizeProperties:
            property = propertyAttr['name']
            cppType = propertyAttr['cpp_type']
            fileContent += self.genObjcGetterImplemenation(cppType, property)
            if 'options' in propertyAttr and 'readonly' not in propertyAttr['options']:
                fileContent += self.genObjcSetterImplemenation(cppType, property)
        fileContent += '\n@end\n'
        filePath = re.sub(r'\.h$', r'.mm', self.objcHeader)
        if force or not os.path.isfile(filePath):
            with open(filePath, 'w+') as fout:
                fout.write(fileContent)
        return fileContent

    def genObjcHeader(self, force = False):
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
        fileContent = \
"""#ifndef %(ifndefMacro)s
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
        filePath = self.objcHeader
        if force or not os.path.isfile(filePath):
            with open(filePath, 'w+') as fout:
                fout.write(fileContent)
        return fileContent

    def genCppFile(self, force = False):
        tab = ' ' * self.tabLen
        fileContent = '#include "%s"\n\n' % (self.cppHeader)
        if self.wrapIn:
            fileContent += '%s\n\n' % (self.wrapIn[0])
        for constructorKey in self.cppConstructors:
            constructorAttr = self.cppConstructors[constructorKey]
            argList = []
            initList = []
            if 'base_class_init' in constructorAttr and constructorAttr['base_class_init']:
                initList += ['%s(%s)' % (base[0], ', '.join(base[1])) for base in constructorAttr['base_class_init']]
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
            codeLines = []
            if 'options' in constructorAttr and constructorAttr['options']:
                if "decode_json" in constructorAttr['options']:
                    codeLines.append(tab + 'decodeJson();')
                elif 'json_object' in constructorAttr['options']:
                    codeLines.append(tab + 'decodeJson(jsonObject);')
                    codeLines.append(tab + 'encodeJson(jsonObject);')
                elif 'encode_json' in constructorAttr['options']:
                    codeLines.append(tab + 'encodeJson();')
            constructorText = '%s::%s(%s)%s {\n%s\n}\n\n' % (self.className, self.className, argListText, initListText, '\n'.join(codeLines))
            fileContent += constructorText
        for access in self.instanceMethods:
            methods = self.instanceMethods[access]
            for method in methods:
                if self.isPredifinedMethod(method):
                    fileContent += self.genPredefinedMethod(method)
                    continue
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
            fileContent += '%s %s::%s%s() {\n%sreturn _%s;\n}\n\n' % (type, self.className, getterPrefix, suffix, tab, property)
            if "options" in propertyAttr and not "readonly" in propertyAttr["options"]:
                if "assign" in propertyAttr["options"]:
                    argumentType = type
                else:
                    argumentType = 'const %s &' % (type)
                fileContent += 'void %s::%s%s(%s %s) {\n%s_%s = %s;\n}\n\n' % (setterPrefix, self.className, suffix, argumentType, property, tab, property, property)
        if self.wrapIn:
            fileContent += '%s\n\n' % (self.wrapIn[1])
        filePath = re.sub(r'\.h$', r'.cpp', self.cppHeader)
        if force or not os.path.isfile(filePath):
            with open(filePath, 'w+') as fout:
                fout.write(fileContent)
        return fileContent

    def genCppHeader(self, force = False):
        ifndefMacro = '__%s__' % (re.sub(r'[\./\/]', r'_', self.cppHeader))
        includeText = '\n'.join(['#include "%s"' % (file) for file in self.includes])
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
                if self.isPredifinedMethod(method):
                    print "predefined %s" % (method)
                    classContent[access].append(predefinedMethods[method])
                    continue
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


        classDeclaration = \
"""#ifndef %(ifndefMacro)s
#define %(ifndefMacro)s
""" % {'ifndefMacro' : ifndefMacro}
        classDeclaration += '%s\n\n' % (includeText)
        if self.wrapIn:
            classDeclaration += '%s\n\n' % (self.wrapIn[0])
        if baseClasses:
            classDeclaration += 'class %s : %s {\n' % (self.className, ', '.join([', '.join(classList) for classList in baseClasses]))
        else:
            classDeclaration += 'class %s {\n' % (self.className)
        for access in ['public', 'protected', 'private']:
            if classContent[access]:
                classDeclaration += '%s:\n%s\n' % (access, '\n'.join(classContent[access]))
        classDeclaration += '};\n\n'
        if self.wrapIn:
            classDeclaration += '%s\n\n' % (self.wrapIn[1])
        classDeclaration += '#endif // %s' % (ifndefMacro)
        filePath = self.cppHeader
        if force or not os.path.isfile(filePath):
            with open(filePath, 'w+') as fout:
                fout.write(classDeclaration)
        return classDeclaration

    def isPredifinedMethod(self, method):
        return method in predefinedMethods

    def genPredefinedMethod(self, method):
        tab = ' ' * self.tabLen
        code = ''
        lines = []
        if method == 'decodeJson':
            for propertyAttr in self.synthesizeProperties:
                property = propertyAttr['name']
                cppType = propertyAttr['cpp_type']
                if cppType == 'std::string':
                    line = tab + '_%s = jsonObject["%s"].GetString();' % (property, property)
                elif cppType == 'bool':
                    line = tab + '_%s = jsonObject["%s"].GetBool();' % (property, property)
                elif cppType == 'long' or cppType == 'time_t':
                    line = tab + '_%s = jsonObject["%s"].GetInt64();' % (property, property)
                elif cppType == 'int':
                    line = tab + '_%s = jsonObject["%s"].GetInt();' % (property, property)
                else:
                    line = tab + '_%s = jsonObject["%s"].Get%s();' % (property, property, cppType)
                lines.append(line)
            code = \
"""
void %(className)s::decodeJson() {
    rapidjson::Document document;
    document.Parse(_json.c_str());
    if (document.IsObject()) {
        decodeJson(document);
    }
}

void %(className)s::decodeJson(const rapidjson::Value & jsonObject) {
%(code)s
}
""" % {'className' : self.className, 'code' : '\n'.join(lines)}
        elif method == 'encodeJson':
            for propertyAttr in self.synthesizeProperties:
                property = propertyAttr['name']
                cppType = propertyAttr['cpp_type']
                if cppType == 'std::string':
                    line = tab + 'JSON_ADD_STRING(jsonObject, "%s", _%s);' % (property, property)
                elif cppType == 'bool':
                    line = tab + 'JSON_ADD_BOOL(jsonObject, "%s", _%s);' % (property, property)
                elif cppType == 'long' or cppType == 'time_t':
                    line = tab + 'JSON_ADD_INT64(jsonObject, "%s", _%s);' % (property, property)
                elif cppType == 'int':
                    line = tab + 'JSON_ADD_INT(jsonObject, "%s", _%s);' % (property, property)
                else:
                    line = tab + 'JSON_ADD_%s(jsonObject, "%s", _%s);;' % (cppType, property, property)
                lines.append(line)
            code = \
"""void %(className)s::encodeJson() {
    rapidjson::Document jsonObject;
    jsonObject.SetObject();
%(code)s
    encodeJson(jsonObject);
}

void %(className)s::encodeJson(const rapidjson::Value & jsonObject) {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    jsonObject.Accept(writer);
    _json = buffer.GetString();
}
""" % {'className' : self.className, 'code' : '\n'.join(lines)}
        return code

modelPath = './XClassModels'
classModels = [os.path.join(modelPath, f) for f in os.listdir(modelPath) if f.endswith('.json') and os.path.isfile(os.path.join(modelPath, f))]
for file in classModels:
    with open(file, 'r') as f:
        s = f.read()
        jsonObj = json.loads(s)
        model = XClassModel(jsonObj)
        model.genCppHeader(True)
        model.genCppFile(True)
        model.genObjcFile(True)
        model.genObjcHeader(True)
