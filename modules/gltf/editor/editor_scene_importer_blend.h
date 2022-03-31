/*************************************************************************/
/*  editor_scene_importer_blend.h                                        */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef EDITOR_SCENE_IMPORTER_BLEND_H
#define EDITOR_SCENE_IMPORTER_BLEND_H

#ifdef TOOLS_ENABLED

#include "editor/import/resource_importer_scene.h"

class Animation;
class Node;

class EditorSceneFormatImporterBlend : public EditorSceneFormatImporter {
	GDCLASS(EditorSceneFormatImporterBlend, EditorSceneFormatImporter);

public:
	enum {
		BLEND_VISIBLE_VISIBLE_ONLY,
		BLEND_VISIBLE_RENDERABLE,
		BLEND_VISIBLE_ALL
	};
	enum {
		BLEND_BONE_INFLUENCES_NONE,
		BLEND_BONE_INFLUENCES_COMPATIBLE,
		BLEND_BONE_INFLUENCES_ALL
	};
	enum {
		BLEND_MODIFIERS_NONE,
		BLEND_MODIFIERS_ALL
	};

	virtual uint32_t get_import_flags() const override;
	virtual void get_extensions(List<String> *r_extensions) const override;
	virtual Node *import_scene(const String &p_path, uint32_t p_flags,
			const Map<StringName, Variant> &p_options, int p_bake_fps,
			List<String> *r_missing_deps, Error *r_err = nullptr) override;
	virtual Ref<Animation> import_animation(const String &p_path, uint32_t p_flags,
			const Map<StringName, Variant> &p_options, int p_bake_fps) override;
	virtual void get_import_options(const String &p_path,
			List<ResourceImporter::ImportOption> *r_options) override;
	virtual Variant get_option_visibility(const String &p_path, const String &p_option,
			const Map<StringName, Variant> &p_options) override;
};

#endif // TOOLS_ENABLED

#endif // EDITOR_SCENE_IMPORTER_BLEND_H