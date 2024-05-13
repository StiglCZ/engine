# python is soo bad. :/, please send help
import bpy
import struct
from bpy.props import BoolProperty, EnumProperty, StringProperty

def exportSST(context, filePath):
    scene = bpy.context.scene
    with open(filePath, "wb") as file:
        for obj in scene.objects:
            if(obj.type == 'LIGHT' or obj.type == 'CAMERA'):
                continue # Skip lights(and camera), as they are uselless
            else:
                pos = obj.location
                rot = obj.rotation_euler
                sca = obj.scale
                # Z+ in blender is Up, but in SST Y- is Up
                data = "\n{:.5f} {:.5f} {:.5f}\n{:.5f} {:.5f} {:.5f}\n{:.5f} {:.5f} {:.5f}\n".format(pos.x, -pos.z, pos.y, rot.x, rot.z, rot.y, sca.x, sca.z, sca.y)
                filename = ''
                try:
                    filename = obj['filename']
                except:
                    filename = "Missing"
                    continue
                if(filename != 'Missing'):
                    file.write((filename + data).encode())
                            
class SSTExporter(bpy.types.Operator):
    bl_idname = "export.sst"
    bl_label = "Export SST"
    filepath: StringProperty(subtype='FILE_PATH')

    def execute(self, context):
        filePath = bpy.path.ensure_ext(self.filepath, ".sst")
        exportSST(context, filePath)
        return {'FINISHED'}

    def invoke(self, context, event):
        if not self.filepath:
            self.filepath = bpy.path.ensure_ext(bpy.data.filepath, ".sst")
        WindowManager = context.window_manager
        WindowManager.fileselect_add(self)
        return {'RUNNING_MODAL'}
    
# Import a normal obj but with a path variable
class SSTImporter(bpy.types.Operator):
    bl_idname = "import.obj"
    bl_label = "Import OBJ"
    filename_ext = ".obj"
    
    filepath: StringProperty(
            name="input file",
            subtype='FILE_PATH'
    )
    filter_glob: StringProperty(
        default="*.obj",
        options={'HIDDEN'},
    )
    
    def execute(self, context):
        bpy.ops.wm.obj_import(filepath=self.filepath)
        bpy.context.active_object['filename'] = self.filepath
        return {'FINISHED'}

    def invoke(self, context, event):
        WindowManager = context.window_manager
        WindowManager.fileselect_add(self)
        return {'RUNNING_MODAL'}
