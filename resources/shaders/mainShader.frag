#version 430 core

layout(location = 0) out vec4 o_color;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	float transparency;
};

const int MAX_POINT_LIGHTS = 16;
const int MAX_SPOT_LIGHTS = 16;

in vec2 v_tex_coord;
in vec3 v_normal;
in vec3 v_position;

struct VSOutput
{
	vec2 TexCoord;
	vec3 Normal;
	vec3 WorldPos;
};

struct BaseLight
{
	vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;
};

struct DirectionalLight
{
	BaseLight Base;
	vec3 Direction;
};

struct Attenuation
{
	float Constant;
	float Linear;
	float Exp;
};

struct PointLight
{
	BaseLight Base;
	vec3 Position;
	Attenuation Atten;
};

struct SpotLight
{
	PointLight Base;
	vec3 Direction;
	float Cutoff;
};

uniform int gNumPointLights = 0;
uniform int gNumSpotLights = 0;
uniform DirectionalLight gDirectionalLight;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];
uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS];
uniform sampler2D diffuse0;
uniform vec3 gEyeWorldPos;
uniform float gMatSpecularIntensity;
uniform float gSpecularPower;
uniform bool has_texture = false;
uniform Material material;
uniform float transparency;
uniform bool lighting_on = true;
uniform bool fog_on = false;
uniform vec3 fog_colour;
uniform int fog_factor_type;
uniform float fog_start = 3.0f;
uniform float fog_end = 15.0f;

in vec4 v_pos;
float rho = 0.15f;

uniform bool colouring_on = false;
uniform vec3 in_colour = vec3(1,0,0);
uniform vec2 texture_scale = vec2(1,1);

vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, VSOutput In)
{
	vec4 AmbientColor;
	if(has_texture)
		AmbientColor = vec4(Light.Color * Light.AmbientIntensity, 1.0f);
	else
		AmbientColor = vec4(Light.Color * Light.AmbientIntensity * material.ambient, 1.0f);
	float DiffuseFactor = dot(In.Normal, -LightDirection);

	vec4 DiffuseColor  = vec4(0, 0, 0, 0);
	vec4 SpecularColor = vec4(0, 0, 0, 0);

	if (DiffuseFactor > 0.0) {
		if(has_texture)
			DiffuseColor = vec4(Light.Color * Light.DiffuseIntensity * DiffuseFactor, 1.0f);
		else
			DiffuseColor = vec4(Light.Color * Light.DiffuseIntensity * DiffuseFactor * material.diffuse, 1.0f);
		vec3 VertexToEye = normalize(gEyeWorldPos - In.WorldPos);
		vec3 LightReflect = normalize(reflect(LightDirection, In.Normal));
		float SpecularFactor = dot(VertexToEye, LightReflect);
		if (SpecularFactor > 0.0) {
			SpecularFactor = pow(SpecularFactor, gSpecularPower);
			if(has_texture)
				SpecularColor = vec4(Light.Color * gMatSpecularIntensity * SpecularFactor, 1.0f);
			else
				SpecularColor = vec4(Light.Color * gMatSpecularIntensity * SpecularFactor * material.specular * material.shininess, 1.0f);
		}
	}

	return (AmbientColor + DiffuseColor + SpecularColor);
}

vec4 CalcDirectionalLight(VSOutput In)
{
	return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, In);
}

vec4 CalcPointLight(PointLight l, VSOutput In)
{
	vec3 LightDirection = In.WorldPos - l.Position;
	float Distance = length(LightDirection);
	LightDirection = normalize(LightDirection);

	vec4 Color = CalcLightInternal(l.Base, LightDirection, In);
	float Attenuation =  l.Atten.Constant +
	l.Atten.Linear * Distance +
	l.Atten.Exp * Distance * Distance;

	return Color / Attenuation;
}

vec4 CalcSpotLight(SpotLight l, VSOutput In)
{
	vec3 LightToPixel = normalize(In.WorldPos - l.Base.Position);
	float SpotFactor = dot(LightToPixel, l.Direction);

	if (SpotFactor > l.Cutoff) {
		vec4 Color = CalcPointLight(l.Base, In);
		return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - l.Cutoff));
	} else {
		return vec4(0,0,0,0);
	}
}

void main()
{
	VSOutput In;
	In.TexCoord = v_tex_coord * texture_scale;
	In.Normal   = normalize(v_normal);
	In.WorldPos = v_position;

	vec4 result;

	if(!lighting_on) {
		if(has_texture) {
			result = texture(diffuse0, In.TexCoord.xy);
			result.w *= transparency;
		} else
			result = vec4(material.ambient, material.transparency);

		// uncomment the following to affect the skybox with the light colour
		//result = light.colour * texture(diffuse0, In.TexCoord.xy);
	} else  {
		vec4 TotalLight = CalcDirectionalLight(In);

		for (int i = 0 ; i < gNumPointLights ; i++) {
			TotalLight += CalcPointLight(gPointLights[i], In);
		}

		for (int i = 0 ; i < gNumSpotLights ; i++) {
			TotalLight += CalcSpotLight(gSpotLights[i], In);
		}

		if (has_texture) {
			result = texture(diffuse0, In.TexCoord.xy) * TotalLight;
			result.w *= transparency;
		} else {
			if (material.transparency < 1.0)
				TotalLight.w = material.transparency;
			result = TotalLight;
		}
	}

	if(fog_on) {
		float d = length(v_pos.xyz);
		float w;
		if(fog_factor_type == 0) {
			if (d < fog_end)
				w = (fog_end - d) / (fog_end-fog_start);
			else
				w = 0;
		} else if (fog_factor_type == 1) {
			w = exp(-(rho*d));
		} else {
			w = exp(-(rho*d)*(rho*d));
		}
		result.rgb = mix(fog_colour, result.rgb, w);
	}

	if(colouring_on) {
		result = result * vec4(in_colour, transparency);
	}

	o_color = result;
}
