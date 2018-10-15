#pragma once

#define MAX_VAR 32

enum {
	v_AIMEnabled,
	v_AIMAutoshoot,
	v_AIMSilent,
	v_AIMEveryone,
	v_AIMNoRecoil,
	v_AIMNoSpread,

	v_ESPEnabled,
	v_ESPBox,
	v_ESPBoxInfo,
	v_ESPEveryone,
	v_ESPLines,

	v_BunnyHop,
	v_AutoStrafe,
	v_pSpeed,
	v_NameStealer,

};


class cFeatures {
	float fnums[MAX_VAR];
	bool vars[MAX_VAR];
	int nums[MAX_VAR];
public:
	cFeatures() {
		for (int i = 0; i < MAX_VAR; i++) {
			fnums[i] = 0;
			nums[i] = 0;
		}
		for (int i = 0; i < MAX_VAR; i++)
			vars[i] = false;

	}
	~cFeatures() {

	}
	void fSetNum(int id, float v) {
		fnums[id] = v;
	}
	void SetNum(int id, int v) {
		nums[id] = v;
	}
	bool bGetValue(int v) {
		return vars[v];
	}
	int iGetValue(int id) {
		return nums[id];
	}

	void SetVar(int id, bool v) {
		vars[id] = v;
	}
};
extern cFeatures *Features;