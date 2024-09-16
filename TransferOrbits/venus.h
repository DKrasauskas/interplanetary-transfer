#pragma once
namespace venus {
	float inclination =  24.46397633556437;
		float ecentricity = 0.006810339650842032;
		float s_major = 108209548790.4671;
		float longitudeOfAscendingNode = 7.981603378781639;
		float argumentOfPeriapsis = 123.7121294282329;
}
namespace earth {
	float inclination = 23.44603795469773;
	float ecentricity = 0.01609636160505683;
	float s_major = 149598261150.4425;
	float longitudeOfAscendingNode = 359.9965004168758;
	float argumentOfPeriapsis = 102.9720683296131;
}

namespace mars {
	float s_major = 227949699961.9763;
	float ecentricity = 0.09326110278323557;
	float inclination = 24.69272426910055;
	float longitudeOfAscendingNode = 3.351911063089117;
	float argumentOfPeriapsis = 332.1022655295414;

}
namespace mercury {
	float s_major = 57908973645.88802;
	float ecentricity = 0.2056187266319207;
	float inclination = 28.60252108855048;	
	float longitudeOfAscendingNode = 10.86541167564728;
	float argumentOfPeriapsis = 66.90371044151551;

}
namespace jupiter {
	float s_major = 778188938659.7554;
	float ecentricity = 0.04872660654702194;
    float inclination = 23.25313306947884;
	float longitudeOfAscendingNode = 3.262077289923354;
	float argumentOfPeriapsis = 10.75642751202877;
}
#define GM 1.32712440018e20
#define R_NORM  149597870700 
#define V_NORM  173611.111

struct orbit {
	glm::vec3 r;
	glm::vec3 v;
	glm::vec3 normal;
	glm::vec3 b1;
	glm::vec3 b2;
	float a;
	float rmag;
};
orbit get(float inclination, float e, float a, float lAN, float AOP) {
	//orbital parameters
	glm::vec3 normal = glm::vec3(-sin(glm::radians(inclination)), 0, cos(glm::radians(inclination)));
	glm::vec3 AN = glm::vec3(sin(glm::radians(lAN)), -cos(glm::radians(lAN)), 0);
	glm::vec3 DND = glm::normalize(glm::cross(AN, normal));
	glm::vec3 periapsis = DND * sin(glm::radians(AOP)) + AN * cos(glm::radians(AOP));
	periapsis = glm::normalize(periapsis);
	glm::vec3 uv = glm::normalize(glm::cross(periapsis, normal));
	float vmag = sqrt(GM * 1 / a * (1 + e) / (1 - e));
	return { periapsis * a * (1 - e),  vmag*uv, normal, periapsis, glm::normalize(-glm::cross(normal, periapsis)), a, 0};
}
void set_annomaly(orbit& orbit, float degrees) {
	 glm::vec3 r = (orbit.b1 * (float)cos(glm::radians(degrees)) + orbit.b2 * sin(glm::radians(degrees))) *(float)sqrt(glm::dot(orbit.r, orbit.r));
	 glm::vec3 v = glm::normalize(glm::cross(r, orbit.normal)) * (float)sqrt(GM * (2 / sqrt(glm::dot(r, r)) - 1 / orbit.a));
	 orbit.r = r;
	 orbit.v = v;
}