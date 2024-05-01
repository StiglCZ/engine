import bpy
import os
from . import operator

bl_info = {
    "name": "Export StiglEngine scenes",
    "author": "Stigl",
    "version": (1, 0, 0),
    "blender": (4, 1, 0),
    "location": "File > Export > Stigl engine scene(.sst)",
    "description": "Adds an export option for stiglengine scenes",
    "category": "Import-Export",
}
def menu_func1(self, context):
    self.layout.operator(operator.SSTExporter.bl_idname, text="Stigl engine scene (.sst)")
def menu_func2(self, context):
    self.layout.operator(operator.SSTImporter.bl_idname, text="SST Object (.obj)")
classes = (
    operator.SSTExporter,
    operator.SSTImporter
)

def register():
    from bpy.utils import register_class
    for cls in classes:
        register_class(cls)
    bpy.types.TOPBAR_MT_file_export.append(menu_func1)
    bpy.types.TOPBAR_MT_file_import.append(menu_func2)


def unregister():
    from bpy.utils import unregister_class
    for cls in reversed(classes):
        unregister_class(cls)
    bpy.types.TOPBAR_MT_file_export.remove(menu_func1)
    bpy.types.TOPBAR_MT_file_import.remove(menu_func2)

if __name__ == "__main__":
    register()
