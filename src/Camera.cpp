#include "main.hpp"


static mat4 infinitePerspectiveFovReverseZLH_ZO(float fov, float width, float height, float zNear) {
    const float h = Mathgl::cot(0.5f * fov);
    const float w = h * height / width;
    mat4 result = Mathgl::zero<mat4>();
    result[0][0] = w;
    result[1][1] = h;
    result[2][2] = 0.0f;
    result[2][3] = 1.0f;
    result[3][2] = zNear;
    return result;
};



void Camera::Init(Application& app, const vec3& position, float fov, const uvec2& size){
    cam_type = SPC_ARCBALL_CAM_TYPE;

    real_cam_angle = dvec3(0.5, 0.5, SPC_UNIVERSE_RADIUS_MM*3.0);
    cam_angle = real_cam_angle;

    near_plane = 0.001f;
    real_forward_dir = vec3(0,0,1);
    real_left_dir = vec3(1,0,0);
    forward_dir = real_forward_dir;
    left_dir = real_left_dir;

    this->eye = -position;
    this->fov = fov;
    app.renderer.InitCamera(app, *this);
    Resize(app, size);

    eye += dvec3(0, 0, SPC_UNIVERSE_RADIUS_MM*3.0);
}



void Camera::Resize(Application& app, const uvec2& size){
    projection = infinitePerspectiveFovReverseZLH_ZO(
        radians(fov), (float)size.x, (float)size.y, near_plane
    );
    resolution = size;
}



void Camera::Update(Application& app){
    // lock mouse
	if (app.hxi.GetKey(app.keyboard, RSI_CTRL_BUTTON, HX_INPUT_KEYSTATE_PRESSED)){
		mouseLocked = !mouseLocked;
		if (mouseLocked){
			app.hxi.HideAndLockCursorToWindow(app.windowBinding);
		}else{
			app.hxi.UnhideAndUnlockCursor();
		}
	}

    // set speed


    if (
        app.hxi.GetKey(app.gamepad, RSI_CAM_TYPE_BUTTON, HX_INPUT_KEYSTATE_RELEASED) ||
        app.hxi.GetKey(app.keyboard, RSI_CAM_TYPE_BUTTON, HX_INPUT_KEYSTATE_RELEASED)
    ){
        cam_type = CamType((cam_type + 1) % SPC_CAM_TYPE_COUNT);
    }

    switch (cam_type){
        case SPC_FLY_CAM_TYPE:
            UpdateFreeMoveCamera(app);
            acceleration = SPC_UNIVERSE_RADIUS_MM * 0.0005;
        break;
        case SPC_ARCBALL_CAM_TYPE:
            UpdateArcballCamera(app);
            acceleration = SPC_UNIVERSE_RADIUS_MM * 3.0;
        break;
    }

    view = Mathgl::lookAtLH(vec3(0), forward_dir, Mathgl::cross(forward_dir, left_dir));

    {
        const auto exponent = StaticUtils::double_extract_exponent(-velocity);
        const auto mantissa = StaticUtils::double_extract_mantissa(-velocity, exponent);
        std::string cam_str("Velocity: "+std::to_string(mantissa));
        if (exponent != 0){ cam_str += "E" + std::string(exponent > 0 ? "+" : "") + std::to_string(exponent); }
        cam_str += "\n";
        cam_str += "Camera type: ";
        cam_str += cam_type == SPC_FLY_CAM_TYPE ? "Fly Camera" : "Arcball Camera";
        app.fontRenderer.RenderText(app, app.robotoMono, vec2(-0.99, 0.9), cam_str, 0.035f);
    }
}


void Camera::UpdateArcballCamera(Application& app){
    dvec3 mp = dvec3(0);

    {
        vec2 delta = vec2(0);
        if (mouseLocked || app.hxi.GetKey(app.mouse, RSI_LMB_BUTTON, HX_INPUT_KEYSTATE_DOWN)){
            delta = app.hxi.GetAnalogState(app.mouse, RSI_MOUSE_DELTA_ANALOGUE).value * 0.005f;
        }
        float zoom = app.hxi.GetAnalogState(app.mouse, RSI_MOUSE_WHEEL_ANALOGUE).value.y * 0.075f;
        mp += vec3(-delta.x, delta.y, -zoom);
    }
    {
        auto delta = app.hxi.GetAnalogState(app.gamepad, RSI_RSTICK_ANALOGUE).value * 0.025f;
        float zoom = app.hxi.GetAnalogState(app.gamepad, RSI_LSTICK_ANALOGUE).value.y * 0.1f;
        mp += vec3(-delta.x, delta.y, -zoom);
    }

    mp.z *= acceleration;

    real_cam_angle += vec3(std::isnan(mp.x) ? 0 : mp.x, std::isnan(mp.y) ? 0 : mp.y, std::isnan(mp.z) ? 0 : mp.z);
    real_cam_angle.z = max(0.0, real_cam_angle.z);
    real_cam_angle.y = clamp(real_cam_angle.y, 0.01, 0.99);
    cam_angle = mix(cam_angle, real_cam_angle, 0.25);

    vec2 r = vec2(cam_angle.x*pi<float>()*2.0f, cam_angle.y*pi<float>());
    vec3 neye = vec3(cos(r.x)*sin(r.y), cos(r.y), sin(r.x)*sin(r.y));
    eye = cam_angle.z * dvec3(neye);

    forward_dir = neye;
    left_dir = -Mathgl::cross(forward_dir, vec3(0,1,0));
    real_forward_dir = forward_dir;
    real_left_dir = left_dir;
}

void Camera::UpdateFreeMoveCamera(Application& app){
    const auto slerp_safe = [](const vec3& start, const vec3& end, const float& a) -> vec3 {
        const auto result = Mathgl::slerp(start, end, a);
        if (Mathgl::any(Mathgl::isnan(result))){ return start; }
        return result;
    };

    float forward=0, left=0, brakes=0;
    float cam_updown=0, cam_leftright=0, cam_roll=0;

    // keyboard
    int btn_forward = app.hxi.GetKey(app.keyboard, RSI_FWD_BUTTON, HX_INPUT_KEYSTATE_DOWN);
    int btn_backward = app.hxi.GetKey(app.keyboard, RSI_BCK_BUTTON, HX_INPUT_KEYSTATE_DOWN);
    forward += float(btn_backward - btn_forward);

    int btn_left = app.hxi.GetKey(app.keyboard, RSI_LEFT_BUTTON, HX_INPUT_KEYSTATE_DOWN);
    int btn_right = app.hxi.GetKey(app.keyboard, RSI_RIGHT_BUTTON, HX_INPUT_KEYSTATE_DOWN);
    left += float(btn_left - btn_right);

    brakes += app.hxi.GetKey(app.keyboard, RSI_BRAKES_BUTTON, HX_INPUT_KEYSTATE_DOWN);

    const auto btn_q = app.hxi.GetKey(app.keyboard, RSI_Q_BUTTON, HX_INPUT_KEYSTATE_DOWN);
    const auto btn_e = app.hxi.GetKey(app.keyboard, RSI_E_BUTTON, HX_INPUT_KEYSTATE_DOWN);
    cam_roll += (int(btn_q) - int(btn_e)) * 0.03f;

    if (mouseLocked || app.hxi.GetKey(app.mouse, RSI_LMB_BUTTON, HX_INPUT_KEYSTATE_DOWN)){
        const vec2 delta = app.hxi.GetAnalogState(app.mouse, RSI_MOUSE_DELTA_ANALOGUE).value;
        cam_leftright += delta.x * 0.005f;
        cam_updown += delta.y * 0.005f;
    }

    // gamepad
    {
        const vec2 rdelta = app.hxi.GetAnalogState(app.gamepad, RSI_RSTICK_ANALOGUE).value;
        cam_leftright += (std::isnan(rdelta.x) ? 0 : rdelta.x) * 0.025f;
        cam_updown += -(std::isnan(rdelta.y) ? 0 : rdelta.y) * 0.025f;

        const vec2 ldelta = app.hxi.GetAnalogState(app.gamepad, RSI_LSTICK_ANALOGUE).value;
        cam_roll += (std::isnan(ldelta.x) ? 0 : ldelta.x) * 0.03125f;
        forward += -(std::isnan(ldelta.y) ? 0 : ldelta.y) * 0.5f;

        brakes += app.hxi.GetKey(app.gamepad, RSI_BRAKES_BUTTON, HX_INPUT_KEYSTATE_DOWN);
    }

    if (brakes){
		velocity *= pow(0.995f, app.delta_time);
	}

    cam_leftright = mod(abs(cam_leftright), Mathgl::pi<float>()*2.0f) * sign(cam_leftright);
    cam_updown = mod(abs(cam_updown), Mathgl::pi<float>()*2.0f) * sign(cam_updown);
    cam_roll = mod(abs(cam_roll), Mathgl::pi<float>()*2.0f) * sign(cam_roll);

    real_forward_dir = Mathgl::rotate(real_forward_dir, cam_updown, left_dir);
    real_left_dir = Mathgl::rotate(real_left_dir, cam_roll, forward_dir);

    vec3 normal = Mathgl::cross(real_forward_dir, real_left_dir);
    real_forward_dir = Mathgl::rotate(real_forward_dir, cam_leftright, normal);
    real_left_dir = Mathgl::rotate(real_left_dir, cam_leftright, normal);

    forward_dir = slerp_safe(forward_dir, real_forward_dir, 0.2f);
    left_dir = slerp_safe(left_dir, real_left_dir, 0.2f);
    forward_spd = mix(forward_spd, forward, 0.2f);
    left_spd = mix(left_spd, left, 0.2f);

    velocity += acceleration * forward_spd;
    eye += dvec3(forward_dir) * velocity;
    eye += dvec3(left_dir * left_spd) * acceleration * 10.0;
}



void Camera::Destroy(Application& app){

}
