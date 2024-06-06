// https://kripken.github.io/emscripten-site/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html

var GameDistributionLibrary = {

    $Context: {
        listener: null,
        activeElementId: null,
    },

    GameDistribution_PlatformInit: function() {
        window["GD_DEFOLD"] = {
            "onEvent": function(event) {
                if ((event.name == "SDK_GAME_START" || event.name == "CONTENT_RESUME_REQUESTED") && Context.activeElementId) {
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
                    stringToUTF8OnStack(event.name),
                    stringToUTF8OnStack(event.message)
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
        if (typeof element === 'undefined' || element == null) {
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
        if (typeof element === 'undefined' || element == null) {
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

    GameDistribution_PlatformShowSendGameEvent: function(levelText, scoreText) {
        if (typeof gdsdk !== 'undefined' && gdsdk.showAd !== 'undefined') {
            var level = parseInt(UTF8ToString(levelText).trim().replace(/[^\d]/g,''));
            var score = parseInt(UTF8ToString(scoreText).trim().replace(/[^\d]/g,''));
            if (typeof level !== 'undefined' && typeof score !== 'undefined') {
                const obj = {
                    "eventName" : "game_event",
                    "data" : {
                        "level" : level,
                        "score" : score
                    }
                }; 
                gdsdk.sendEvent(obj)
                    .then(() => console.info('sendEvent resolved.'))
                    .catch(error => console.error(error));
            }
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
