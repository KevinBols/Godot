/*************************************************************************/
/*  xr_interface.h                                                       */
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

#ifndef XR_INTERFACE_H
#define XR_INTERFACE_H

#include "core/math/camera_matrix.h"
#include "core/os/thread_safe.h"
#include "servers/xr_server.h"

// forward declaration
struct BlitToScreen;

/**
	The XR interface is a template class on top of which we build interface to different AR, VR and tracking SDKs.
	The idea is that we subclass this class, implement the logic, and then instantiate a singleton of each interface
	when Godot starts. These instances do not initialize themselves but register themselves with the AR/VR server.

	If the user wants to enable AR/VR the choose the interface they want to use and initialize it.

	Note that we may make this into a fully instantiable class for GDNative support.
*/

class XRInterface : public RefCounted {
	GDCLASS(XRInterface, RefCounted);

public:
	enum Capabilities { /* purely meta data, provides some info about what this interface supports */
		XR_NONE = 0, /* no capabilities */
		XR_MONO = 1, /* can be used with mono output */
		XR_STEREO = 2, /* can be used with stereo output */
		XR_QUAD = 4, /* can be used with quad output (not currently supported) */
		XR_VR = 8, /* offers VR support */
		XR_AR = 16, /* offers AR support */
		XR_EXTERNAL = 32 /* renders to external device */
	};

	enum TrackingStatus { /* tracking status currently based on AR but we can start doing more with this for VR as well */
		XR_NORMAL_TRACKING,
		XR_EXCESSIVE_MOTION,
		XR_INSUFFICIENT_FEATURES,
		XR_UNKNOWN_TRACKING,
		XR_NOT_TRACKING
	};

	enum PlayAreaMode { /* defines the mode used by the XR interface for tracking */
		XR_PLAY_AREA_UNKNOWN, /* Area mode not set or not available */
		XR_PLAY_AREA_3DOF, /* Only support orientation tracking, no positional tracking, area will center around player */
		XR_PLAY_AREA_SITTING, /* Player is in seated position, limited positional tracking, fixed guardian around player */
		XR_PLAY_AREA_ROOMSCALE, /* Player is free to move around, full positional tracking */
		XR_PLAY_AREA_STAGE, /* Same as roomscale but origin point is fixed to the center of the physical space, XRServer.center_on_hmd disabled */
	};

protected:
	_THREAD_SAFE_CLASS_

	static void _bind_methods();

public:
	/** general interface information **/
	virtual StringName get_name() const = 0;
	virtual uint32_t get_capabilities() const = 0;

	bool is_primary();
	void set_primary(bool p_is_primary);

	virtual bool is_initialized() const = 0; /* returns true if we've initialized this interface */
	virtual bool initialize() = 0; /* initialize this interface, if this has an HMD it becomes the primary interface */
	virtual void uninitialize() = 0; /* deinitialize this interface */

	/** input and output **/

	virtual PackedStringArray get_suggested_tracker_names() const; /* return a list of likely/suggested tracker names */
	virtual PackedStringArray get_suggested_pose_names(const StringName &p_tracker_name) const; /* return a list of likely/suggested action names for this tracker */
	virtual TrackingStatus get_tracking_status() const; /* get the status of our current tracking */
	virtual void trigger_haptic_pulse(const String &p_action_name, const StringName &p_tracker_name, double p_frequency, double p_amplitude, double p_duration_sec, double p_delay_sec = 0); /* trigger a haptic pulse */

	/** specific to VR **/
	virtual bool supports_play_area_mode(XRInterface::PlayAreaMode p_mode); /* query if this interface supports this play area mode */
	virtual XRInterface::PlayAreaMode get_play_area_mode() const; /* get the current play area mode */
	virtual bool set_play_area_mode(XRInterface::PlayAreaMode p_mode); /* change the play area mode, note that this should return false if the mode is not available */
	virtual PackedVector3Array get_play_area() const; /* if available, returns an array of vectors denoting the play area the player can move around in */

	/** specific to AR **/
	virtual bool get_anchor_detection_is_enabled() const;
	virtual void set_anchor_detection_is_enabled(bool p_enable);
	virtual int get_camera_feed_id();

	/** rendering and internal **/

	virtual Size2 get_render_target_size() = 0; /* returns the recommended render target size per eye for this device */
	virtual uint32_t get_view_count() = 0; /* returns the view count we need (1 is monoscopic, 2 is stereoscopic but can be more) */
	virtual Transform3D get_camera_transform() = 0; /* returns the position of our camera for updating our camera node. For monoscopic this is equal to the views transform, for stereoscopic this should be an average */
	virtual Transform3D get_transform_for_view(uint32_t p_view, const Transform3D &p_cam_transform) = 0; /* get each views transform */
	virtual CameraMatrix get_projection_for_view(uint32_t p_view, double p_aspect, double p_z_near, double p_z_far) = 0; /* get each view projection matrix */

	// note, external color/depth/vrs texture support will be added here soon.

	virtual void process() = 0;
	virtual void pre_render(){};
	virtual bool pre_draw_viewport(RID p_render_target) { return true; }; /* inform XR interface we are about to start our viewport draw process */
	virtual Vector<BlitToScreen> post_draw_viewport(RID p_render_target, const Rect2 &p_screen_rect) = 0; /* inform XR interface we finished our viewport draw process */
	virtual void end_frame(){};

	virtual void notification(int p_what){};

	XRInterface();
	~XRInterface();
};

VARIANT_ENUM_CAST(XRInterface::Capabilities);
VARIANT_ENUM_CAST(XRInterface::TrackingStatus);
VARIANT_ENUM_CAST(XRInterface::PlayAreaMode);

#endif // XR_INTERFACE_H