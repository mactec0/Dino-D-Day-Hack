#pragma once
#include "vector.h"
#include "Color.h" 
#include "tools.h"
#include <iostream> 


typedef void*	(*Interface)(char* Name, int Return);

void* GetInterface(char* _Module, char* _Interface) {
	Interface TempInterface = (Interface)GetProcAddress(GetModuleHandleSafe(_Module), "CreateInterface");
	return (void*)TempInterface(_Interface, 0);
}

template< typename Function > Function CallVirtual(void* class_pointer, int id) {
	uint32_t * vmt = *(uint32_t**)(class_pointer);
	return (Function)(vmt[id]);
}

#define	FL_ONGROUND				(1<<0)	// At rest / on the ground
#define FL_DUCKING				(1<<1)	// Player flag -- Player is fully crouched
#define	FL_WATERJUMP			(1<<2)	// player jumping out of water
#define FL_ONTRAIN				(1<<3) // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN				(1<<4)	// Indicates the entity is standing in rain
#define FL_FROZEN				(1<<5) // Player is frozen for 3rd person camera
#define FL_ATCONTROLS			(1<<6) // Player can't move, but keeps key inputs for controlling another entity
#define	FL_CLIENT				(1<<7)	// Is a player
#define FL_FAKECLIENT			(1<<8)	// Fake client, simulated server side; don't send network messages to them
// NON-PLAYER SPECIFIC (i.e., not used by GameMovement or the client .dll ) -- Can still be applied to players, though
#define	FL_INWATER				(1<<9)	// In water

// NOTE if you move things up, make sure to change this value
#define PLAYER_FLAG_BITS		10

#define	FL_FLY					(1<<10)	// Changes the SV_Movestep() behavior to not need to be on ground
#define	FL_SWIM					(1<<11)	// Changes the SV_Movestep() behavior to not need to be on ground (but stay in water)
#define	FL_CONVEYOR				(1<<12)
#define	FL_NPC					(1<<13)
#define	FL_GODMODE				(1<<14)
#define	FL_NOTARGET				(1<<15)
#define	FL_AIMTARGET			(1<<16)	// set if the crosshair needs to aim onto the entity
#define	FL_PARTIALGROUND		(1<<17)	// not all corners are valid
#define FL_STATICPROP			(1<<18)	// Eetsa static prop!		
#define FL_GRAPHED				(1<<19) // worldgraph has this ent listed as something that blocks a connection
#define FL_GRENADE				(1<<20)
#define FL_STEPMOVEMENT			(1<<21)	// Changes the SV_Movestep() behavior to not do any processing
#define FL_DONTTOUCH			(1<<22)	// Doesn't generate touch functions, generates Untouch() for anything it was touching when this flag was set
#define FL_BASEVELOCITY			(1<<23)	// Base velocity has been applied this frame (used to convert base velocity into momentum)
#define FL_WORLDBRUSH			(1<<24)	// Not moveable/removeable brush entity (really part of the world, but represented as an entity for transparency or something)
#define FL_OBJECT				(1<<25) // Terrible name. This is an object that NPCs should see. Missiles, for example.
#define FL_KILLME				(1<<26)	// This entity is marked for death -- will be freed by game DLL
#define FL_ONFIRE				(1<<27)	// You know...
#define FL_DISSOLVING			(1<<28) // We're dissolving!
#define FL_TRANSRAGDOLL			(1<<29) // In the process of turning into a client side ragdoll.
#define FL_UNBLOCKABLE_BY_PLAYER (1<<30) // pusher that can't be blocked by the player

typedef unsigned long HFont;
typedef unsigned long HTexture;
typedef unsigned long HCursor;

class CGlobalVarsBase {
public:
	float realtime;
	int framecount;
	float absoluteframetime;
	float curtime;
	float frametime;
	int maxclients;
	int tickcount;
	float interval_per_tick;
	float interpolation_amount;
	int    simThicksThisFrame;
	int    network_protocol;
};
extern CGlobalVarsBase*GlobalVars;

#define IN_ATTACK		(1 << 0)
#define IN_JUMP			(1 << 1)
#define IN_DUCK			(1 << 2)
#define IN_FORWARD		(1 << 3)
#define IN_BACK			(1 << 4)
#define IN_USE			(1 << 5)
#define IN_CANCEL		(1 << 6)
#define IN_LEFT			(1 << 7)
#define IN_RIGHT		(1 << 8)
#define IN_MOVELEFT		(1 << 9)
#define IN_MOVERIGHT	(1 << 10)
#define IN_ATTACK2		(1 << 11)
#define IN_RUN			(1 << 12)
#define IN_RELOAD		(1 << 13)
#define IN_ALT1			(1 << 14)
#define IN_ALT2			(1 << 15)
#define IN_SCORE		(1 << 16)   // Used by client.dll for when scoreboard is held down
#define IN_SPEED		(1 << 17)	// Player is holding the speed key
#define IN_WALK			(1 << 18)	// Player holding walk key
#define IN_ZOOM			(1 << 19)	// Zoom key for HUD zoom
#define IN_WEAPON1		(1 << 20)	// weapon defines these bits
#define IN_WEAPON2		(1 << 21)	// weapon defines these bits
#define IN_BULLRUSH		(1 << 22)
#define IN_GRENADE1		(1 << 23)	// grenade 1
#define IN_GRENADE2		(1 << 24)	// grenade 2
#define	IN_ATTACK3		(1 << 25)

class CUserCmd {
public:
	char pad_0x0000[0x4];
	int command_number;
	int tick_count;
	Vector viewangles;
	float fowardmove;
	float sidemove;
	float upmove;
	int buttons;
	char pad_0x0028[0xC];
	int random_seed;
	short	mousedx;
	short	mousedy;
	char pad_0x0038[0x1C];
};

enum eSurfaceVTableIndex {
	VTABLE_DRAWSETCOLOR = 14,
	VTABLE_DRAWSETCOLOR_RGBA = 15,
	VTABLE_DRAWFILLEDRECT = 16,
	VTABLE_DRAWOUTLINEDRECT = 18,
	VTABLE_DRAWLINE = 19,
	VTABLE_DRAWSETTEXTFONT = 23,
	VTABLE_DRAWSETTEXTCOLOR = 24,
	VTABLE_DRAWSETTEXTCOLOR_RGBA = 25,
	VTABLE_DRAWSETTEXTPOS = 26,
	VTABLE_DRAWPRINTTEXT = 28,
	VTABLE_CREATEFONT = 71,
	VTABLE_SETFONTGLYPHSET = 72,
	VTABLE_GETTEXTSIZE = 79,
	VTABLE_DRAWOUTLINEDCIRCLE = 162
};

class ISurface {
public:

	void DrawSetColor(cColor Color) {
		typedef void(__thiscall* _DrawSetColor)(void*, cColor);
		return CallVirtual<_DrawSetColor>(this, 13)(this, Color);
	}

	void DrawFilledRect(int X, int Y, int W, int H) {
		typedef void(__thiscall* _DrawFilledRect)(void*, int, int, int, int);
		return CallVirtual<_DrawFilledRect>(this, 15)(this, X, Y, W, H);
	}

	void DrawOutlinedRect(int X, int Y, int W, int H) {
		typedef void(__thiscall* DrawOutlinedRect)(void*, int, int, int, int);
		return CallVirtual<DrawOutlinedRect>(this, 17)(this, X, Y, W, H);
	}

	void DrawLine(int x0, int y0, int x1, int y1) {
		typedef void(__thiscall* _DrawLine)(void*, int, int, int, int);
		return CallVirtual<_DrawLine>(this, 18)(this, x0, y0, x1, y1);
	}

	void DrawSetTextFont(int Font) {
		typedef void(__thiscall* _DrawSetTextFont)(void*, int);
		return CallVirtual<_DrawSetTextFont>(this, 22)(this, Font);
	}

	void DrawSetTextColor(cColor Color) {
		typedef void(__thiscall* _DrawSetTextColor)(void*, cColor);
		return CallVirtual<_DrawSetTextColor>(this, 23)(this, Color);
	}

	void DrawSetTextPos(int X, int Y) {
		typedef void(__thiscall* _DrawSetTextPos)(void*, int, int);
		return CallVirtual<_DrawSetTextPos>(this, 25)(this, X, Y);
	}

	void DrawPrintText(wchar_t* _Input, int Length, int Type) {
		typedef void(__thiscall* _DrawPrintText)(void*, wchar_t*, int, int);
		return CallVirtual<_DrawPrintText>(this, 27)(this, _Input, Length, Type);
	}


	HFont CreateFont() {
		typedef HFont(__thiscall* _CreateFont)(void*);
		return CallVirtual<_CreateFont>(this, 69)(this);
	}

	bool  SetFontGlyphSet(HFont font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin = 0, int nRangeMax = 0) {
		typedef bool(__thiscall* _SetFontGlyphSet)(void*, HFont, const char *, int, int, int, int, int, int, int);
		return CallVirtual<_SetFontGlyphSet>(this, 70)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, nRangeMin, nRangeMax);
	}
	bool GetTextSize(int Font, const wchar_t* _Input, int& Wide, int& Tall) {
		typedef bool(__thiscall* _GetTextSize)(void*, int, const wchar_t*, int&, int&);
		return CallVirtual< _GetTextSize >(this, 77)(this, Font, _Input, Wide, Tall);
	}
	void SurfaceGetCursorPos(int &x, int &y) {
		typedef void(__thiscall* _SurfaceGetCursorPos)(void*, int&, int&);
		return CallVirtual< _SurfaceGetCursorPos >(this, 100)(this, x, y);
	}


	void SetCursorAlwaysVisible(bool visible) {
		typedef void(__thiscall* _SetCursorAlwaysVisible)(void*, bool);
		return CallVirtual<_SetCursorAlwaysVisible>(this, 57)(this, visible);
	}
	void DrawOutlinedCircle(int x, int y, int radius, int segments) {
		typedef void(__thiscall* _DrawOutlinedCircle)(void*, int, int, int, int);
		return CallVirtual<_DrawOutlinedCircle>(this, 103)(this, x, y, radius, segments);
	}

}; extern ISurface * Surface;



class IPanel {
public:

	const char* GetName(int Panel) {
		typedef const char*(__thiscall* _GetName)(void*, int);
		return CallVirtual< _GetName >(this, 36)(this, Panel);
	}

}; extern IPanel * Panel;


enum FontDrawType_t {
	FONT_DRAW_DEFAULT = 0,
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,
	FONT_DRAW_TYPE_COUNT = 2,
};

enum EFontFlags {
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};


class Matrix3x4_t {
public:
	float Matrix[16];
};
typedef float matrix3x4[3][4];


struct PlayerInfo_t {
private:
public:
	int m_nXuidLow;
	int m_nXuidHigh;
	char Name[128];
	int m_nUserID;
	char m_szSteamID[33];
	UINT m_nSteam3ID;
	char m_szFriendsName[128];
	bool m_bIsFakePlayer;
	bool m_bIsHLTV;
	DWORD m_dwCustomFiles[4];
	BYTE m_FilesDownloaded;
private:
	int __pad1;
};

class IVEngineClient {
public:


	void GetScreenSize(int& Width, int& Height) {
		typedef void(__thiscall* _GetScreenSize)(void*, int&, int&);
		return CallVirtual< _GetScreenSize >(this, 5)(this, Width, Height);
	}
	void GetPlayerInfo(int Index, PlayerInfo_t* _Info) {
		typedef void(__thiscall* _GetPlayerInfo)(void*, int, PlayerInfo_t*);
		return CallVirtual<_GetPlayerInfo>(this, 8)(this, Index, _Info);
	}
	int GetLocalPlayer() {
		typedef int(__thiscall* _GetLocalPlayer)(void*);
		return CallVirtual<_GetLocalPlayer>(this, 12)(this);
	}
	int GetPlayerForUserID(int ID) {
		typedef int(__thiscall* _GetPlayerForUserID)(void*, int);
		return CallVirtual<_GetPlayerForUserID>(this, 9)(this, ID);
	}
	void SetViewAngles(Vector& va) {
		typedef void(__thiscall* _SetViewAngles)(void*, Vector&);
		return CallVirtual<_SetViewAngles>(this, 19)(this, va);
	}

	float GetLastTimeStamp(void) {
		typedef float(__thiscall* _GetLastTimeStamp)(PVOID);
		return CallVirtual< _GetLastTimeStamp >(this, 14)(this);
	}

	int GetMaxClients() {
		typedef int(__thiscall* _GetMaxClients)(void*);
		return CallVirtual<_GetMaxClients>(this, 20)(this);
	}
	bool IsInGame() {
		typedef bool(__thiscall* _IsInGame)(void*);
		return CallVirtual<_IsInGame>(this, 25)(this);
	}
	bool IsConnected() {
		typedef bool(__thiscall* _IsConnected)(void*);
		return CallVirtual<_IsConnected>(this, 26)(this);
	}

	const Matrix3x4_t& GetMatrix() {
		typedef const Matrix3x4_t& (__thiscall* _GetMatrix)(void*);
		return CallVirtual< _GetMatrix>(this, 37)(this);
	}

	int ClientCMD(const char* command) {
		typedef int(__thiscall* _func)(void*, const char* command);
		return CallVirtual<_func>(this, 104)(this, command);
	}



}; extern IVEngineClient * EngineClient;


typedef  unsigned long EHANDLE;


enum lifestate_t {
	LIFE_ALIVE,
	LIFE_DYING,
	LIFE_DEAD,
	LIFE_RESPAWNABLE,
	LIFE_DISCARDBODY,
};

class IClientEntityList {
public:

	virtual void PAD0();
	virtual void PAD1();
	virtual void PAD2();

	virtual void* GetClientEntity(int Index);
	virtual void* GetClientEntityFromHandle(EHANDLE _Handle);

	virtual int NumberOfEntities(bool NonNetworkable);
	virtual int GetHighestEntityIndex();
	virtual void SetMaxEntities(int Max);
	virtual void GetMaxEntities();

}; extern IClientEntityList * ClientEntityList;


class model_t;

enum Hitboxes {
	HITBOX_HEAD,
	HITBOX_NECK,
	HITBOX_LOWER_NECK,
	HITBOX_PELVIS,
	HITBOX_BODY,
	HITBOX_THORAX,
	HITBOX_CHEST,
	HITBOX_UPPER_CHEST,
	HITBOX_RIGHT_THIGH,
	HITBOX_LEFT_THIGH,
	HITBOX_RIGHT_CALF,
	HITBOX_LEFT_CALF,
	HITBOX_RIGHT_FOOT,
	HITBOX_LEFT_FOOT,
	HITBOX_RIGHT_HAND,
	HITBOX_LEFT_HAND,
	HITBOX_RIGHT_UPPER_ARM,
	HITBOX_RIGHT_FOREARM,
	HITBOX_LEFT_UPPER_ARM,
	HITBOX_LEFT_FOREARM,
	HITBOX_MAX
};

struct mstudiobbox_t {
	int     bone;
	int     group;
	Vector  bbmin;
	Vector  bbmax;
	int     szhitboxnameindex;
	int        m_iPad01[3];
	float        m_flRadius;
	int        m_iPad02[4];

	char* GetHitboxName(void) {
		if (szhitboxnameindex == 0)
			return "";

		return ((char*)this) + szhitboxnameindex;
	}
};
struct mstudiobone_t {
	int                                     sznameindex;
	inline char * const GetName(void) const { return ((char *)this) + sznameindex; }
	int                                     parent;
	int                                     bonecontroller[6];

	Vector                          pos;
	float                           quat[4];
	Vector                          rot;
	Vector                          posscale;
	Vector                          rotscale;

	matrix3x4                     poseToBone;
	float                           qAlignment[4];
	int                                     flags;
	int                                     proctype;
	int                                     procindex;              // procedural rule
	mutable int                     physicsbone;    // index into physically simulated bone
	inline void *           GetProcedure() const { if (procindex == 0) return NULL; else return  (void *)(((BYTE *)this) + procindex); };
	int                                     surfacepropidx; // index into string tablefor property name
	inline char * const GetSurfaceProps(void) const { return ((char *)this) + surfacepropidx; }
	int                                     contents;               // See BSPFlags.h for the contents flags

	int                                     unused[8];              // remove as appropriate
};
struct mstudiohitboxset_t {
	int                                             sznameindex;
	inline char* const              GetName(void) const { return ((char*)this) + sznameindex; }
	int                                             numhitboxes;
	int                                             hitboxindex;
	inline mstudiobbox_t*   GetHitbox(int i) const { return (mstudiobbox_t*)(((BYTE*)this) + hitboxindex) + i; };
};
struct studiohdr_t {
	int                                     id;
	int                                     version;

	int                                     checksum;

	char                            name[64];
	int                                     length;


	Vector                          eyeposition;

	Vector                          illumposition;

	Vector                          hull_min;
	Vector                          hull_max;

	Vector                          view_bbmin;
	Vector                          view_bbmax;

	int                                     flags;

	int                                     numbones;
	int                                     boneindex;

	inline mstudiobone_t *GetBone(int i) const { return (mstudiobone_t *)(((BYTE *)this) + boneindex) + i; };
	//	inline mstudiobone_t *pBone(int i) const { Assert(i >= 0 && i < numbones); return (mstudiobone_t *)(((BYTE *)this) + boneindex) + i; };

	int                                     numbonecontrollers;
	int                                     bonecontrollerindex;

	int                                     numhitboxsets;
	int                                     hitboxsetindex;

	mstudiohitboxset_t* GetHitboxSet(int i) const {
		return (mstudiohitboxset_t*)(((BYTE*)this) + hitboxsetindex) + i;
	}

	inline mstudiobbox_t* GetHitbox(int i, int set) const {
		mstudiohitboxset_t const* s = GetHitboxSet(set);

		if (!s)
			return NULL;

		return s->GetHitbox(i);
	}

	inline int GetHitboxCount(int set) const {
		mstudiohitboxset_t const* s = GetHitboxSet(set);

		if (!s)
			return 0;

		return s->numhitboxes;
	}

	int                                     numlocalanim;
	int                                     localanimindex;

	int                                     numlocalseq;
	int                                     localseqindex;

	mutable int                     activitylistversion;
	mutable int                     eventsindexed;

	int                                     numtextures;
	int                                     textureindex;

	int                                     numcdtextures;
	int                                     cdtextureindex;

	int                                     numskinref;
	int                                     numskinfamilies;
	int                                     skinindex;

	int                                     numbodyparts;
	int                                     bodypartindex;

	int                                     numlocalattachments;
	int                                     localattachmentindex;

	int                                     numlocalnodes;
	int                                     localnodeindex;
	int                                     localnodenameindex;

	int                                     numflexdesc;
	int                                     flexdescindex;

	int                                     numflexcontrollers;
	int                                     flexcontrollerindex;

	int                                     numflexrules;
	int                                     flexruleindex;

	int                                     numikchains;
	int                                     ikchainindex;

	int                                     nummouths;
	int                                     mouthindex;

	int                                     numlocalposeparameters;
	int                                     localposeparamindex;

	int                                     surfacepropindex;

	int                                     keyvalueindex;
	int                                     keyvaluesize;


	int                                     numlocalikautoplaylocks;
	int                                     localikautoplaylockindex;

	float                           mass;
	int                                     contents;

	int                                     numincludemodels;
	int                                     includemodelindex;

	mutable void            *virtualModel;

	int                                     szanimblocknameindex;
	int                                     numanimblocks;
	int                                     animblockindex;

	mutable void            *animblockModel;

	int                                     bonetablebynameindex;

	void                            *pVertexBase;
	void                            *pIndexBase;

	BYTE                            constdirectionallightdot;

	BYTE                            rootLOD;

	BYTE                            numAllowedRootLODs;

	BYTE                            unused[1];

	int                                     unused4;

	int                                     numflexcontrollerui;
	int                                     flexcontrolleruiindex;
	float                           flVertAnimFixedPointScale;
	int                                     unused3[1];
	int                                     studiohdr2index;
	int                                     unused2[1];
};

struct studiohwdata_t;
typedef unsigned short 	StudioDecalHandle_t;
typedef unsigned short ModelInstanceHandle_t;

class IClientRenderable;

struct ModelRenderInfo_t {
	Vector origin;
	Vector angles;
	IClientRenderable *pRenderable;
	const model_t *pModel;
	const Matrix3x4_t *pModelToWorld;
	const Matrix3x4_t *pLightingOffset;
	const Vector *pLightingOrigin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	ModelInstanceHandle_t instance;
	ModelRenderInfo_t() {
		pModelToWorld = NULL;
		pLightingOffset = NULL;
		pLightingOrigin = NULL;
	}
};

struct DrawModelState_t {
	studiohdr_t*			m_pStudioHdr;
	studiohwdata_t*			m_pStudioHWData;
	void*		m_pRenderable;
	const Matrix3x4_t		*m_pModelToWorld;
	StudioDecalHandle_t		m_decals;
	int						m_drawFlags;
	int						m_lod;
};

class DVariant {
public:
	union {
		float	m_Float;
		long	m_Int;
		char	*m_pString;
		void	*m_pData;
		float	m_Vector[3];
	};
};

struct RecvTable;
struct RecvProp {
	char					*m_pVarName;
	int						m_RecvType;
	int						m_Flags;
	int						m_StringBufferSize;
	bool					m_bInsideArray;
	const void				*m_pExtraData;
	RecvProp				*m_pArrayProp;
	void*					m_ArrayLengthProxy;
	void*					m_ProxyFn;
	void*					m_DataTableProxyFn;
	RecvTable				*m_pDataTable;
	int						m_Offset;
	int						m_ElementStride;
	int						m_nElements;
	const char				*m_pParentArrayPropName;
};

class CRecvProxyData {
public:
	const RecvProp	*m_pRecvProp;
	char _pad[4];
	DVariant		m_Value;
	int				m_iElement;
	int				m_ObjectID;
};


struct RecvTable {
	RecvProp* m_pProps;
	int	m_nProps;
	void* unused;
	const char* m_pNetTableName;
	bool __pad0[2];
};


struct ClientClass {
private:
public:
	void*			m_pCreateFn;
	void*			m_pCreateEventFn;
	const char* m_pNetworkName;
	RecvTable* m_pRecvTable;
	ClientClass* m_pNext;
	int m_ClassID;
};

class CHLClient {
public:
	ClientClass * GetAllClasses() {
		//VRESULT(ClientClass*, 8, Args())(); 
		typedef ClientClass*(__thiscall* _GetAllClasses)(void*);
		return CallVirtual<_GetAllClasses>(this, 8)(this);
	}
};
extern CHLClient* Client;;


class IVModelInfoClient {
public:
	void* GetModel(int Index) {
		typedef void*(__thiscall* _GetModel)(PVOID, int);
		return CallVirtual< _GetModel >(this, 1)(this, Index);
	}

	int GetModelIndex(const char* Filename) {
		typedef int(__thiscall* GetModelNameFn)(PVOID, const char*);
		return CallVirtual< GetModelNameFn >(this, 2)(this, Filename);
	}
	const char* GetModelName(const model_t* model) {
		typedef const char* (__thiscall* GetModelNameFn)(PVOID, const model_t*);
		return CallVirtual< GetModelNameFn >(this, 3)(this, model);
	}

	studiohdr_t* GetStudioModel(const model_t* mod) {
		typedef studiohdr_t* (__thiscall* GetStudiomodelFn)(PVOID, const model_t*);
		return CallVirtual< GetStudiomodelFn >(this, 30)(this, mod);

	}
};


class IClientNetWorkable {
public:
	ClientClass * GetClientClass() {
		typedef ClientClass*(__thiscall* OriginalFn)(void*);
		return  CallVirtual<OriginalFn>(this, 2)(this);
	}

	int GetIndex() {
		typedef int(__thiscall* OriginalFn)(void*);
		return  CallVirtual<OriginalFn>(this, 10)(this);
	}
};


class CBaseEntity {

public:

	IClientNetWorkable * GetNetworkable() {
		return *(IClientNetWorkable**)(this + 0x08);
	}

	unsigned int GetTickBase() {
		return *(unsigned int*)(this + 0x168C);
	}

	Vector GetBoneOrigin(int i) {
		matrix3x4 boneMatrix[128];
		if (this->SetupBones(boneMatrix, 128, 0x00000100, 0.f))
		{
			return Vector(boneMatrix[i][0][3], boneMatrix[i][1][3], boneMatrix[i][2][3]);
		}
		return Vector(0, 0, 0);
	}

	bool SetupBones(matrix3x4 *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime) {
		void* renderable = this->GetClientRenderable();
		typedef bool(__thiscall* oSetupBones)(void*, matrix3x4*, int, int, float);//102
		return CallVirtual< oSetupBones>(renderable, 13)(renderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}


	void* const GetClientRenderable() {
		return (void*)(this + 0x04);
	}


	int GetHealth() {
		return *(int*)(this + 0xF4);
	}
	int GetMaxHealth() {
		return *(int*)(this + 0xF8);
	}


	BYTE GetLifeState() {
		return *(BYTE*)(this + 0x257);
	}


	int GetFlags() {
		return *(int*)(this + 0xfc);
	}


	EHANDLE GetActiveWeaponHandle() {
		return *(EHANDLE*)(this + 0x1224);//m_hActiveWeapon
	}

	bool GetDormant() {
		return *(bool*)(this + 0xE9);
	}


	Vector GetOrigin() {
		return *(Vector*)(this + 0x130);
	}

	Vector GetVelocityVector() {
		return *(Vector*)(this + 0x10c);
	}


	int GetTeam() {
		return *(int*)(this + 0xEC); //m_iTeamNum
	}

	Vector GetPunch() {
		return *(Vector *)(this + 0x1320);//m_Local + m_vecPunchAngle 
	}

	Vector GetEyePosition() {
		Vector vecPos = *(Vector*)(this + 0x100) + GetOrigin();
		vecPos += this->GetVelocityVector()*GlobalVars->interval_per_tick;
		return vecPos;
	}

};

class C_BaseCombatWeapon :public CBaseEntity {

public:

	float NextAttack() {
		return *(float*)(this + 0x0CC0);//m_flNextPrimaryAttack
	}



	int WeaponID() {
		return *(int*)(this + 0x32F0);
	}

	float GetInaccuracy() {
		typedef float(__thiscall* _GetInaccuracy)(PVOID);
		return CallVirtual< _GetInaccuracy>(this, 393)(this);
	}

};


class CInput {
public:
	CUserCmd * GetUserCMD(int a1, int sequence_number) {
		typedef CUserCmd *(__thiscall* OriginalFn)(void*, int, int);
		return CallVirtual<OriginalFn>(this, 8)(this, a1, sequence_number);
	}
};


typedef unsigned int CRC32_t;
class CVerifiedUserCmd {
public:
	CUserCmd	m_cmd;
	CRC32_t		m_crc;
};




class CPrediction {
public:
	void SetupMove(CBaseEntity *player, CUserCmd *ucmd, PVOID empty, PVOID moveData) {
		typedef void(__thiscall* OriginalFn)(PVOID, CBaseEntity *player, CUserCmd *ucmd, PVOID empty, PVOID moveData);
		CallVirtual<OriginalFn>(this, 19)(this, player, ucmd, NULL, moveData);
	}
	void FinishMove(CBaseEntity *player, CUserCmd *ucmd, PVOID moveData) {
		typedef void(__thiscall* OriginalFn)(PVOID, CBaseEntity *player, CUserCmd *ucmd, PVOID moveData);
		CallVirtual<OriginalFn>(this, 20)(this, player, ucmd, moveData);
	}


};

class CGameMovement {
public:
	void ProcessMovement(CBaseEntity *pPlayer, PVOID moveData) {
		typedef void(__thiscall* OriginalFn)(PVOID, CBaseEntity *pPlayer, PVOID moveData);
		CallVirtual<OriginalFn>(this, 1)(this, pPlayer, moveData);
	}
};



class IMoveHelper {
public:
	void SetHost(CBaseEntity *pPlayer) {
		typedef void(__thiscall* OriginalFn)(void*, CBaseEntity *pPlayer);
		CallVirtual<OriginalFn>(this, 0)(this, pPlayer);
	}
};




struct cplane_t {
	Vector  normal;
	float   dist;
	BYTE    type;
	BYTE    signbits;
	BYTE    pad[2];
};

struct csurface_t {
	const char*             name;
	short                   surfaceProps;
	unsigned short          flags;
};


class CBaseTrace {
public:

public:
	Vector			startpos;				// start position
	Vector			endpos;					// final position
	cplane_t		plane;					// surface normal at impact
	float			fraction;				// time completed, 1.0 = didn't hit anything
	int				contents;				// contents on other side of surface hit
	unsigned short	dispFlags;				// displacement flags for marking surfaces with data
	bool			allsolid;				// if true, plane is not valid
	bool			startsolid;				// if true, the initial point was in a solid area

	CBaseTrace() {}

};

class CGameTrace : public CBaseTrace {
public:
	bool DidHitWorld() const;
	bool DidHitNonWorldEntity() const;
	int GetEntityIndex() const;
	bool DidHit() const;
	bool IsVisible() const;
public:
	float			fractionleftsolid;	// time we left a solid, only valid if we started in solid
	csurface_t		surface;			// surface hit (impact surface)
	int				hitgroup;			// 0 == generic, non-zero is specific body part
	short			physicsbone;		// physics bone hit by trace in studio
	unsigned short	worldSurfaceIndex;	// Index of the msurface2_t, if applicable
	CBaseEntity 	*m_pEnt;
	int			hitbox;					// box hit by trace in studio
	CGameTrace() { }
private:
	//CGameTrace(const CGameTrace& vOther);
};



typedef unsigned int uint;

enum TraceType_t;

typedef CGameTrace trace_t;

enum TraceType_t {
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

class ITraceFilter {
public:
	virtual bool ShouldHitEntity(CBaseEntity* pEntity, int contentsMask) = 0;
	virtual TraceType_t	GetTraceType() const = 0;
};


class CTraceFilter : public ITraceFilter {
public:

	CBaseEntity * m_pPassEnt;
	int type;
	CTraceFilter(CBaseEntity* pPassEntity) {
		m_pPassEnt = pPassEntity;
		type = TRACE_EVERYTHING;
	}

	CTraceFilter() {
	}
	virtual TraceType_t GetTraceType() const {
		return (TraceType_t)type;
	}
	bool ShouldHitEntity(CBaseEntity* pHandleEntity, int contentsMask) {
		return !(pHandleEntity == m_pPassEnt);
	}

};



class Ray_t {
public:
	__declspec(align(16)) Vector m_Start;
	__declspec(align(16)) Vector  m_Delta;
	__declspec(align(16)) Vector  m_StartOffset;
	__declspec(align(16)) Vector  m_Extents;

	const Matrix3x4_t*    m_WorldAxisTransform;


	bool m_IsRay;
	bool m_IsSwept;

	Ray_t(void)
		: m_WorldAxisTransform(0) {

	}


	void Init(Vector& vecStart, Vector& vecEnd) {
		m_Delta = Vector(vecEnd - vecStart);

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_StartOffset = Vector(0, 0, 0);
		m_Start = vecStart;
		m_WorldAxisTransform = NULL;
		m_Extents = Vector(0, 0, 0);
		m_IsRay = true;

	}
};


Vector GetSpread(int seed, C_BaseCombatWeapon *pWeapon) {

	typedef void(__cdecl* RandomSeed_t)(int);
	static RandomSeed_t pRandomSeed = 0;

	typedef float(__cdecl* RandomFloat_t)(float, float);
	static RandomFloat_t pRandomFloat = 0;

	if (!pRandomSeed) {
		pRandomSeed = (RandomSeed_t)(GetProcAddress(GetModuleHandle("vstdlib"), "RandomSeed"));
		if (!pRandomSeed)
			return Vector(0, 0, 0);
	}

	if (!pRandomFloat) {
		pRandomFloat = (RandomFloat_t)(GetProcAddress(GetModuleHandle("vstdlib"), "RandomFloat"));
		if (!pRandomFloat)
			return Vector(0, 0, 0);
	}
	pRandomSeed(seed + 1);
	float fSpreadY = pRandomFloat(-0.5f, 0.5f);
	float r1 = pRandomFloat(-0.5f, 0.5f);
	float fSpreadX = r1 + fSpreadY;
	fSpreadY = pRandomFloat(-0.5f, 0.5f);
	float r2 = pRandomFloat(-0.5f, 0.5f);
	fSpreadY = r2 + fSpreadY;
	float InAccuracy = pWeapon->GetInaccuracy();
	return Vector(fSpreadX*InAccuracy, fSpreadY*InAccuracy, 0);
}

void CompestateSpread(CUserCmd *cmd, C_BaseCombatWeapon * pWeapon) {
	int seed = cmd->random_seed & 255;
	Vector spread = GetSpread(seed, pWeapon) * -1;
	Vector vecAim, v = cmd->viewangles;
	vecAim = v.Forward() + (v.Right() * spread.x) + (v.Up() * spread.y);
	vecAim.NormalizeInPlace();
	cmd->viewangles = vecAim.toAngles();
}

class IEngineTrace {
public:
	void TraceRay(const Ray_t& ray, uint fMask, ITraceFilter *pTraceFilter, trace_t *pTrace) {
		typedef void(__thiscall* _TraceRay)(void*, const Ray_t&, uint, ITraceFilter*, trace_t *);
		return CallVirtual<_TraceRay>(this, 5)(this, ray, fMask, pTraceFilter, pTrace);
	}
};

extern CGameMovement *GameMovement;
extern IEngineTrace *EngineTrace;
extern CInput * Input;


bool GetVisible(Vector& vAbsStart, Vector& vAbsEnd, CBaseEntity* pEntity, CBaseEntity* pLocal) {
	trace_t tr;
	Ray_t ray;
	CTraceFilter  filter(pLocal);
	ray.Init(vAbsStart, vAbsEnd);
	EngineTrace->TraceRay(ray, 0x46004003, &filter, &tr);
	return (tr.m_pEnt == pEntity || tr.fraction > 0.97f);
}

typedef int(__thiscall* GetChecksum_t)(void *ptr);
extern GetChecksum_t GetChecksum;

