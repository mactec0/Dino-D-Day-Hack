#pragma once 
#include <vector>
#include "Render.h"
#include "KeyManger.h"
#include "Features.h"

enum {
	TMisc,
	TAIM,
	TVisuals,
};

enum {
	AUTOSTRAFE_OFF,
	AUTOSTRAFE_SILENT,
	AUTOSTRAFE_PERFECT,
};


enum {
	TYPE_FLOAT,
	TYPE_BOOL,
	TYPE_INT,
	TYPE_STR,
	TYPE_STRLIST,
	TYPE_KEY,
};

class cEntry {
protected:
	int iType;

public:
	bool bValue;
	float fValue;
	int iValue;
	std::string text;
	std::string *List;
	int TabID;
	int ID;
	virtual void Increment() = 0;
	virtual void Decrement() = 0;
	virtual void Switch() = 0;
	virtual int GetType() = 0;
	virtual int iGetValue() = 0;
	virtual bool bGetValue() = 0;
	virtual float fGetValue() = 0;
};

class cEntryBool :public cEntry {
public:
	cEntryBool(std::string text, int id, bool bValue, int TabID) {
		this->text = text;
		this->ID = id;
		this->bValue = bValue;
		this->TabID = TabID;
		iType = TYPE_BOOL;
		iValue = 0;
		fValue = 0;
	}
	void Switch() {
		bValue = !bValue;
		Features->SetVar(ID, bValue);
	}
	void Decrement() { Switch(); }
	void Increment() { Switch(); }
	int iGetValue() { return iValue; }
	bool bGetValue() { return bValue; }
	float fGetValue() { return fValue; }
	int GetType() { return iType; }
};

class cEntryInt :public cEntry {
	int Min, Max;
	int moveV;
public:
	cEntryInt(std::string text, int id, int iValue, int iMin, int iMax, int iMove, int TabID) {
		this->text = text;
		this->ID = id;
		this->iValue = iValue;
		this->TabID = TabID;
		iType = TYPE_INT;
		bValue = 0;
		fValue = 0;
		Max = iMax;
		Min = iMin;
		moveV = iMove;
	}
	void Switch() {	}
	void Decrement() {
		iValue -= moveV;
		Clamp();
		Features->SetNum(ID, iValue);
	}
	void Increment() {
		iValue += moveV;
		Clamp();
		Features->SetNum(ID, iValue);
	}
	void Clamp() {
		if (iValue > Max) {
			iValue = Max;
		}
		if (iValue < Min) {
			iValue = Min;
		}
	}
	int iGetValue() { return iValue; }
	bool bGetValue() { return bValue; }
	float fGetValue() { return fValue; }
	int GetType() { return iType; }
};

class cEntryKey :public cEntry {
public:
	cEntryKey(std::string text, int id, int iValue, int TabID) {
		this->text = text;
		this->ID = id;
		this->iValue = iValue;
		this->TabID = TabID;
		iType = TYPE_KEY;
		bValue = 0;
		fValue = 0;
	}
	void Switch() {	}
	void Decrement() {
	}
	void Increment() {
	}
	void Clamp() {

	}
	int iGetValue() { return iValue; }
	bool bGetValue() { return bValue; }
	float fGetValue() { return fValue; }
	int GetType() { return iType; }
	void SetValue(int v) { iValue = v; }
};


class cEntryStrList :public cEntry {
	int Min, Max;
	int moveV;
public:
	cEntryStrList(std::string text, int id, int iValue, int iMax, int TabID) {
		this->text = text;
		this->ID = id;
		this->iValue = iValue;
		this->TabID = TabID;
		iType = TYPE_STRLIST;
		bValue = 0;
		fValue = 0;
		Max = iMax;
		Min = 0;
		moveV = 1;
	}
	void Switch() {	}
	void Decrement() {
		iValue -= moveV;
		Clamp();
		Features->SetNum(ID, iValue);
	}
	void Increment() {
		iValue += moveV;
		Clamp();
		Features->SetNum(ID, iValue);
	}
	void Clamp() {
		if (iValue >= Max) {
			iValue = Max - 1;
		}
		if (iValue < Min) {
			iValue = Min;
		}
	}
	int iGetValue() {
		Clamp();
		return iValue;
	}
	bool bGetValue() { return bValue; }
	float fGetValue() { return fValue; }
	int GetType() { return iType; }
};

struct sTab {
	std::string text;
	int ID;
	bool bOpen;
	sTab(std::string text, int id, bool bOpen) {
		this->text = text;
		this->ID = id;
		this->bOpen = bOpen;
	}

};

struct sValueBool {
	std::string text;
	int TabID;
	int ID;
	bool value;
	sValueBool(std::string text, int id, bool bValue, int TabID) {
		this->text = text;
		this->ID = id;
		this->value = bValue;
		this->TabID = TabID;
	}
};

struct sValueINT {
	std::string text;
	int ID;
	int value;
	int iMin, iMax;
	sValueINT(std::string text, int id, int bValue, int iMin, int iMax) {
		this->text = text;
		this->ID = id;
		this->value = bValue;
		this->iMin = iMin;
		this->iMax = iMax;
	}
};


struct sValueStringList {
	std::string text;
	std::string *List;
	int ID;
	int value;
	int iMin, iMax;
	sValueStringList(std::string text, int id, int bValue, int iMin, int iMax) {
		this->text = text;
		this->ID = id;
		this->value = bValue;
		this->iMin = iMin;
		this->iMax = iMax;
	}
};


class cGuiLast {

	int IncrementID;
	int DecrementID;
	int SwitchID;

	std::vector<cEntry*> vEntries;


	int currentID = 0;
	bool bChangeCurrent = false;
	std::vector<sTab*> vTab;
public:

	cGuiLast() {
		IncrementID = -1;
		DecrementID = -1;
		SwitchID = -1;

		vTab.push_back(new sTab("Aimbot", TAIM, 0));
		vTab.push_back(new sTab("Visuals", TVisuals, 0));
		vTab.push_back(new sTab("Misc", TMisc, 0));

		vEntries.push_back(new cEntryBool("Enabled", v_AIMEnabled, false, TAIM));
		vEntries.push_back(new cEntryBool("Autoshoot", v_AIMAutoshoot, false, TAIM));
		vEntries.push_back(new cEntryBool("Silent", v_AIMSilent, false, TAIM));
		vEntries.push_back(new cEntryBool("Everyone", v_AIMEveryone, false, TAIM));
		vEntries.push_back(new cEntryBool("NoRecoil", v_AIMNoRecoil, false, TAIM));
		vEntries.push_back(new cEntryBool("NoSpread", v_AIMNoSpread, false, TAIM));




		vEntries.push_back(new cEntryBool("Enabled", v_ESPEnabled, false, TVisuals));
		vEntries.push_back(new cEntryBool("Box", v_ESPBox, false, TVisuals));
		vEntries.push_back(new cEntryBool("Info", v_ESPBoxInfo, false, TVisuals));
		vEntries.push_back(new cEntryBool("Everyone", v_ESPEveryone, false, TVisuals));
		vEntries.push_back(new cEntryBool("Lines", v_ESPLines, false, TVisuals));




		vEntries.push_back(new cEntryBool("BunnyHop", v_BunnyHop, false, TMisc));
		cEntryStrList *StrafeType = new cEntryStrList("AutoStrafe", v_AutoStrafe, 0, 3, TMisc);
		StrafeType->List = new std::string[3];
		StrafeType->List[AUTOSTRAFE_OFF] = "Off";
		StrafeType->List[AUTOSTRAFE_SILENT] = "Silent";
		StrafeType->List[AUTOSTRAFE_PERFECT] = "Perfect";
		vEntries.push_back(StrafeType);
		vEntries.push_back(new cEntryInt("pSpeed", v_pSpeed, 0, 0, 15, 1, TMisc));
		vEntries.push_back(new cEntryBool("NameStealer", v_NameStealer, false, TMisc));





	}

	int GetDrawListID() {
		int DrawID = 0;

		for (int i = 0; i < vTab.size(); i++) {
			DrawID++;
			if (vTab[i]->bOpen) {
				for (int j = 0; j < vEntries.size(); j++) {
					if (vTab[i]->ID == vEntries[j]->TabID) {
						DrawID++;
					}
				}
			}
		}
		return DrawID;
	}
	void Control() {

		if (KeyManger->KeyClicked(KEY_UP))
			currentID--;
		if (KeyManger->KeyClicked(KEY_DOWN))
			currentID++;

		int DrawMax = GetDrawListID();

		if (DrawMax <= currentID)
			currentID = DrawMax - 1;
		if (currentID < 0)
			currentID = 0;

		if (KeyManger->KeyClicked(KEY_LEFT) || KeyManger->KeyClicked(KEY_RIGHT) || KeyManger->KeyClicked(KEY_RETURN)) {
			bChangeCurrent = true;
		}


		if (KeyManger->KeyClicked(KEY_RETURN)) {
			SwitchID = currentID;
		}
		else if (KeyManger->KeyClicked(KEY_RIGHT)) {
			IncrementID = currentID;
		}
		else if (KeyManger->KeyClicked(KEY_LEFT)) {
			DecrementID = currentID;
		}



	}


	void DrawBackGround(int x, int y, int w, int h) {
		Render::DrawFilledRect(x, y, w, h, cColor(0, 0, 0, 220));
		Render::DrawFilledRect(x + 1, y + 1, w - 2, h - 2, cColor(60, 60, 60, 255));
		Render::DrawFilledRect(x + 3, y + 3, w - 6, h - 6, cColor(40, 40, 40, 255));
		Render::DrawFilledRect(x + 6, y + 6, w - 12, h - 12, cColor(60, 60, 60, 255));
		Render::DrawFilledRect(x + 7, y + 7, w - 14, h - 14, cColor(25, 25, 25, 255));
	}

	void Draw() {
		int StartY = 200;
		int StartX = 50;
		int X = StartX;
		int Y = StartY;
		const int tHeight = 18;

		int DrawID = 0;
		int DrawMax = GetDrawListID();

		if (DrawMax <= currentID)
			currentID = DrawMax - 1;
		if (currentID < 0)
			currentID = 0;

		DrawBackGround(StartX, StartY, 200, (DrawMax + 1)*tHeight);

		X += 10;
		Y += 10;


		for (int i = 0; i < vTab.size(); i++) {
			std::string strTab = vTab[i]->text.c_str();
			if (bChangeCurrent&&currentID == DrawID) {
				vTab[i]->bOpen = !vTab[i]->bOpen;
			}

			if (vTab[i]->bOpen)
				strTab += " - ";
			else
				strTab += " + ";

			if (currentID == DrawID)
				Render::Printf(X, Y + DrawID * tHeight, cColor(80, 30, 160, 255), 0, (char*)strTab.c_str());
			else
				Render::Printf(X, Y + DrawID * tHeight, cColor(255, 255, 255, 255), 0, (char*)strTab.c_str());


			DrawID++;

			if (vTab[i]->bOpen) {
				for (int j = 0; j < vEntries.size(); j++) {
					if (vTab[i]->ID == vEntries[j]->TabID) {
						int StringWidth;
						std::string EntryStr = vEntries[j]->text;
						EntryStr += ":";
						if (currentID == DrawID)
							StringWidth = Render::Printf(X + 8, Y + DrawID * tHeight, cColor(70, 25, 145, 255), 0, (char*)EntryStr.c_str());
						else
							StringWidth = Render::Printf(X + 8, Y + DrawID * tHeight, cColor(200, 200, 200, 255), 0, (char*)EntryStr.c_str());

						switch (vEntries[j]->GetType()) {
						case TYPE_BOOL:
							if (vEntries[j]->bGetValue())
								Render::Printf(X + 14 + StringWidth, Y + DrawID * tHeight, cColor(20, 170, 40, 255), 0, "Yes");
							else
								Render::Printf(X + 14 + StringWidth, Y + DrawID * tHeight, cColor(190, 20, 20, 255), 0, "No");

							break;
						case TYPE_INT:
							Render::Printf(X + 14 + StringWidth, Y + DrawID * tHeight, cColor(20, 180, 200, 255), 0, "%d", vEntries[j]->iGetValue());
							break;
						case TYPE_STRLIST:
							Render::Printf(X + 14 + StringWidth, Y + DrawID * tHeight, cColor(230, 230, 10, 255), 0, "%s", vEntries[j]->List[vEntries[j]->iGetValue()].c_str());
							break;
						case TYPE_KEY:
							if (currentID == DrawID) {
								if (GetAsyncKeyState(VK_BACK))
								{
									vEntries[j]->iValue = -1;
									Features->SetNum(vEntries[j]->ID, -1);
								}
								else {
									for (int k = 5; k < 0xA3; k++) {
										if (k < 0x25 || k>0x28)
											if (k != 0x20 && k != 0x57 && k != 0x53 && k != 0x41 && k != 0x44)
												if (GetAsyncKeyState(k)) {
													Features->SetNum(vEntries[j]->ID, k);
													vEntries[j]->iValue = k;
												}
									}
								}
							}
							if (vEntries[j]->iGetValue() == -1)
								Render::Printf(X + 14 + StringWidth, Y + DrawID * tHeight, cColor(120, 5, 200, 255), 0, "...");
							else
								Render::Printf(X + 14 + StringWidth, Y + DrawID * tHeight, cColor(120, 5, 200, 255), 0, "%d", vEntries[j]->iGetValue());
							break;
						}
						if (DrawID == IncrementID) {
							vEntries[j]->Increment();
						}
						if (DrawID == DecrementID) {
							vEntries[j]->Decrement();
						}
						if (DrawID == SwitchID) {
							vEntries[j]->Switch();
						}

						DrawID++;
					}
				}
			}
		}


		IncrementID = -1;
		DecrementID = -1;
		SwitchID = -1;
		bChangeCurrent = false;
	}



}; extern cGuiLast *GuiList;