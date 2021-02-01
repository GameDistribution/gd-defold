// https://kripken.github.io/emscripten-site/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html

var GameDistributionLibrary = {

    $Context: {
        listener: null,
        activeElementId: null,
    },

    GameDistribution_PlatformInit: function(gameId, debug) {
        window["GD_DEFOLD"] = {
            "onEvent": function(event) {
                if (event.name == "SDK_GAME_START" && Context.activeElementId) {
                    var element = document.getElementById(Context.activeElementId);
                    if (element) {
                        element.focus();
                        Context.activeElementId = null;
                    }
                }
                var listener = Context.listener;
                if (!listener) {
                    console.log("No listener set");
                    return;
                }
                {{{ makeDynCall("vii", "listener") }}} (
                    allocate(intArrayFromString(event.name), "i8", ALLOC_STACK),
                    allocate(intArrayFromString(event.message), "i8", ALLOC_STACK)
                );
            }
        };
    },

    GameDistribution_PlatformSetEventListener: function(listener) {
        Context.listener = listener;
    },

    GameDistribution_PlatformShowDisplayAd: function(containerId) {
        var elementId = UTF8ToString(containerId);
        var element = document.getElementById(elementId);
        if (typeof element === 'undefined') {
            console.log("Unable to find element with id " + elementId);
            return;
        }
        element.style.display = "block";
        if (typeof gdsdk !== 'undefined' && gdsdk.showAd !== 'undefined') {
            gdsdk.showAd(gdsdk.AdType.Display, { containerId: elementId })
                .then(() => console.info('showAd(AdType.Display) resolved.'))
                .catch(error => console.error(error));
        }
    },

    GameDistribution_PlatformHideDisplayAd: function(containerId) {
        var elementId = UTF8ToString(containerId);
        var element = document.getElementById(elementId);
        if (typeof element === 'undefined') {
            console.log("Unable to find element with id " + elementId);
            return;
        }
        element.style.display = "none";
    },

    GameDistribution_PlatformShowInterstitialAd: function() {
        if (typeof gdsdk !== 'undefined' && gdsdk.showAd !== 'undefined') {
            var activeElement = document.activeElement;
            if (activeElement) {
                Context.activeElementId = document.activeElement.id;
            }
            gdsdk.showAd(gdsdk.AdType.Interstitial)
                .then(() => console.info('showAd(AdType.Interstitial) resolved.'))
                .catch(error => console.error(error));
        }
    },

    GameDistribution_PlatformShowRewardedAd: function() {
        if (typeof gdsdk !== 'undefined' && gdsdk.showAd !== 'undefined') {
            var activeElement = document.activeElement;
            if (activeElement) {
                Context.activeElementId = document.activeElement.id;
            }
            gdsdk.showAd(gdsdk.AdType.Rewarded)
                .then(() => console.info('showAd(AdType.Rewarded) resolved.'))
                .catch(error => console.error(error));
        }
    },

    GameDistribution_PlatformOpenConsole: function() {
        if (typeof gdsdk !== 'undefined' && gdsdk.openConsole !== 'undefined') {
            gdsdk.openConsole();
        }
    }
};

autoAddDeps(GameDistributionLibrary, "$Context");

mergeInto(LibraryManager.library, GameDistributionLibrary);
