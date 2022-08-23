#include "gamedistribution.h"
#include <dmsdk/sdk.h>

#define LIB_NAME "GameDistribution"
#define MODULE_NAME "gdsdk"
#define DLIB_LOG_DOMAIN LIB_NAME

#if defined(DM_PLATFORM_HTML5)

struct GameDistribution
{
	GameDistribution()
	{
		memset(this, 0, sizeof(*this));
	}
	dmScript::LuaCallbackInfo* m_Callback;
};

static GameDistribution g_GameDistribution;

static void lua_setfieldstringstring(lua_State* L, const char* key, const char* value)
{
	int top = lua_gettop(L);
	lua_pushstring(L, value);
	lua_setfield(L, -2, key);
	assert(top == lua_gettop(L));
}


static void GameDistribution_OnEventListener(const char* event, const char* message)
{
	dmLogInfo("GameDistribution_OnEventListener %s msg: %s", event, message);
	if (g_GameDistribution.m_Callback == 0)
	{
		return;
	}

	lua_State* L = dmScript::GetCallbackLuaContext(g_GameDistribution.m_Callback);

	DM_LUA_STACK_CHECK(L, 0);

	if (!dmScript::IsCallbackValid(g_GameDistribution.m_Callback))
	{
		g_GameDistribution.m_Callback = 0;
		return;
	}

	if (!dmScript::SetupCallback(g_GameDistribution.m_Callback))
	{
		dmScript::DestroyCallback(g_GameDistribution.m_Callback);
		g_GameDistribution.m_Callback = 0;
		return;
	}

	lua_pushstring(L, event);
	lua_pushstring(L, message);

	dmScript::PCall(L, 3, 0);

	dmScript::TeardownCallback(g_GameDistribution.m_Callback);
}

static int GameDistribution_Init(lua_State* L, dmConfigFile::HConfig config)
{
	DM_LUA_STACK_CHECK(L, 0);

	int32_t debug = dmConfigFile::GetInt(config, "gamedistribution.debug", 0);
	const char* gameId = dmConfigFile::GetString(config, "gamedistribution.game_id", "");

	GameDistribution_PlatformInit(gameId, debug);
	return 0;
}

static int GameDistribution_SetListener(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 0);

	if (g_GameDistribution.m_Callback)
	{
		dmScript::DestroyCallback(g_GameDistribution.m_Callback);
	}

	g_GameDistribution.m_Callback = dmScript::CreateCallback(L, 1);

	GameDistribution_PlatformSetEventListener((OnEventCallback)GameDistribution_OnEventListener);
	return 0;
}

static int GameDistribution_ShowRewardedAd(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 0);
	GameDistribution_PlatformShowRewardedAd();
	return 0;
}

static int GameDistribution_ShowDisplayAd(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 0);
	const char* containerId = luaL_checkstring(L, 1);
	GameDistribution_PlatformShowDisplayAd(containerId);
	return 0;
}

static int GameDistribution_HideDisplayAd(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 0);
	const char* containerId = luaL_checkstring(L, 1);
	GameDistribution_PlatformHideDisplayAd(containerId);
	return 0;
}

static int GameDistribution_ShowInterstitialAd(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 0);
	GameDistribution_PlatformShowInterstitialAd();
	return 0;
}

static int GameDistribution_ShowSendGameEvent(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 0);
	const char* levelText = luaL_checkstring(L, 1);
	const char* scoreText = luaL_checkstring(L, 2);
	GameDistribution_PlatformShowSendGameEvent(levelText,scoreText);
	return 0;
}

static int GameDistribution_OpenConsole(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 0);
	GameDistribution_PlatformOpenConsole();
	return 0;
}


static const luaL_reg Module_methods[] =
{
	{"set_listener", GameDistribution_SetListener},
	{"show_rewarded_ad", GameDistribution_ShowRewardedAd},
	{"show_interstitial_ad", GameDistribution_ShowInterstitialAd},
	{"show_display_ad", GameDistribution_ShowDisplayAd},
	{"hide_display_ad", GameDistribution_HideDisplayAd},
	{"send_game_event", GameDistribution_ShowSendGameEvent},
	{"open_console", GameDistribution_OpenConsole},
	{0, 0}
};

static void LuaInit(lua_State* L)
{
	int top = lua_gettop(L);
	luaL_register(L, MODULE_NAME, Module_methods);

	lua_setfieldstringstring(L, "SDK_READY", "SDK_READY");
	lua_setfieldstringstring(L, "SDK_ERROR", "SDK_ERROR");
	lua_setfieldstringstring(L, "SDK_GAME_START", "SDK_GAME_START");
	lua_setfieldstringstring(L, "SDK_GAME_PAUSE", "SDK_GAME_PAUSE");
	lua_setfieldstringstring(L, "SDK_REWARDED_WATCH_COMPLETE", "SDK_REWARDED_WATCH_COMPLETE");
	lua_setfieldstringstring(L, "SDK_GDPR_TRACKING", "SDK_GDPR_TRACKING");
	lua_setfieldstringstring(L, "SDK_GDPR_TARGETING", "SDK_GDPR_TARGETING");
	lua_setfieldstringstring(L, "SDK_GDPR_THIRD_PARTY", "SDK_GDPR_THIRD_PARTY");

	lua_setfieldstringstring(L, "AD_SDK_MANAGER_READY", "AD_SDK_MANAGER_READY");
	lua_setfieldstringstring(L, "AD_SDK_CANCELED", "AD_SDK_CANCELED");

	// IMA SDK events
	lua_setfieldstringstring(L, "AD_ERROR", "AD_ERROR");
	lua_setfieldstringstring(L, "AD_BREAK_READY", "AD_BREAK_READY");
	lua_setfieldstringstring(L, "AD_METADATA", "AD_METADATA");
	lua_setfieldstringstring(L, "ALL_ADS_COMPLETED", "ALL_ADS_COMPLETED");
	lua_setfieldstringstring(L, "CLICK", "CLICK");
	lua_setfieldstringstring(L, "COMPLETE", "COMPLETE");
	lua_setfieldstringstring(L, "CONTENT_PAUSE_REQUESTED", "CONTENT_PAUSE_REQUESTED");
	lua_setfieldstringstring(L, "CONTENT_RESUME_REQUESTED", "CONTENT_RESUME_REQUESTED");
	lua_setfieldstringstring(L, "DURATION_CHANGE", "DURATION_CHANGE");
	lua_setfieldstringstring(L, "FIRST_QUARTILE", "FIRST_QUARTILE");
	lua_setfieldstringstring(L, "IMPRESSION", "IMPRESSION");
	lua_setfieldstringstring(L, "INTERACTION", "INTERACTION");
	lua_setfieldstringstring(L, "LINEAR_CHANGED", "LINEAR_CHANGED");
	lua_setfieldstringstring(L, "LOADED", "LOADED");
	lua_setfieldstringstring(L, "LOG", "LOG");
	lua_setfieldstringstring(L, "MIDPOINT", "MIDPOINT");
	lua_setfieldstringstring(L, "PAUSED", "PAUSED");
	lua_setfieldstringstring(L, "RESUMED", "RESUMED");
	lua_setfieldstringstring(L, "SKIPPABLE_STATE_CHANGED", "SKIPPABLE_STATE_CHANGED");
	lua_setfieldstringstring(L, "SKIPPED", "SKIPPED");
	lua_setfieldstringstring(L, "STARTED", "STARTED");
	lua_setfieldstringstring(L, "THIRD_QUARTILE", "THIRD_QUARTILE");
	lua_setfieldstringstring(L, "USER_CLOSE", "USER_CLOSE");
	lua_setfieldstringstring(L, "VOLUME_CHANGED", "VOLUME_CHANGED");
	lua_setfieldstringstring(L, "VOLUME_MUTED", "VOLUME_MUTED");

	lua_pop(L, 1);
	assert(top == lua_gettop(L));
}
#endif

dmExtension::Result AppInitializeGameDistributionExtension(dmExtension::AppParams* params)
{
	return dmExtension::RESULT_OK;
}

dmExtension::Result InitializeGameDistributionExtension(dmExtension::Params* params)
{
	#if defined(DM_PLATFORM_HTML5)
	LuaInit(params->m_L);
	GameDistribution_Init(params->m_L, params->m_ConfigFile);
	#endif
	return dmExtension::RESULT_OK;
}

dmExtension::Result AppFinalizeGameDistributionExtension(dmExtension::AppParams* params)
{
	#if defined(DM_PLATFORM_HTML5)
	if (g_GameDistribution.m_Callback != 0)
	{
		dmScript::DestroyCallback(g_GameDistribution.m_Callback);
		g_GameDistribution.m_Callback = 0;
	}
	#endif
	return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeGameDistributionExtension(dmExtension::Params* params)
{
	return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(GameDistribution, LIB_NAME, AppInitializeGameDistributionExtension, AppFinalizeGameDistributionExtension, InitializeGameDistributionExtension, 0, 0, FinalizeGameDistributionExtension)
