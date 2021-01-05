# GameDistribution SDK for Defold
The repository containing the GameDistribution SDK for Defold games. This allows you to display advertisements and other solutions available to games published within the GameDistribution network.


## Registering as a developer
Before you can start using GameDistribution you need to register as a developer:

https://developer.gamedistribution.com/register/developer/

When you have registered as a developer you also need to register your game in the GameDistribution developer portal:

https://developer.gamedistribution.com/games

When you have registered the game head to the Upload tab and copy the Game ID:

![](/docs/gameid.png)


## Installation
You can integrate GameDistribution in both new games and in existing games. Please follow the instructions below depending on your situation.


### Creating a new game
If you are creating a new game it is recommended that you use the GameDistribution project template from the Defold editor Welcome screen. The GameDistribution template includes the [GameDistribution SDK extension](https://github.com/GameDistribution/gd-defold):

![GameDistribution template](/docs/gamedistribution-template.png)

Add your Game ID to the GameDistribution section of the **game.project** file:

![Adding game id game.project](/docs/adding-gameid.png)


### Configuring an existing game
If you wish to use GameDistribution with an existing game you can open the **game.project** file and in the [Dependencies field in the Project section](https://defold.com/manuals/project-settings/#dependencies) add:

```
https://github.com/GameDistribution/gd-defold/archive/master.zip
```

Open the **game.project** file using a text editor and add a new section with your Game ID:

```
[gamedistribution]
game_id = ADD YOUR GAME ID HERE
```


## Usage
To use the GameDistribution SDK in a Defold game you need to first set up a listener function to receive events from the GameDistribution SDK. The listener function will receive events when an ad is shown and the game should be paused and when an ad is closed and the game should be resumed.

```Lua
gdsdk.set_listener(function(self, event, message)
	print(event, message)
	if event == gdsdk.SDK_GAME_PAUSE then
		-- pause your game
	elseif event == gdsdk.SDK_GAME_START then
		-- resume your game
	end
end)
```

Once you have the listener functions set up you can start showing ads. The GameDistribution SDK supports Rewarded Ads, Interstitial Ads and Display Ads.


### Showing a Rewarded ad
Use [Rewarded Ads](https://blog.gamedistribution.com/rewarded-ads-are-here/) to give the user premium content, extra lives, in-game currency and so on. You show a Rewarded Ad like this:

```
gdsdk.show_rewarded_ad()
```


### Showing an Interstitial Ad
Use Interstitial Ads during screen transitions, between sessions and in other situations where there is a natural pause in game play. You show an Interstitial Ad like this:

```
gdsdk.show_interstitial_ad()
```


### Showing a Display Ad
Use Display Ads (also known as a banner ad) at your discretion but make sure to not cover any of your game content. You need to manually add a `<div>` where the Display Ad will be shown. Example:

```html
<!-- center and anchor to bottom of page -->
<div style="position: absolute; bottom: 0px; left: 50%;">
	<div id="canvas-ad" style="width: 728px; height:90px; margin-left: -50%;"/>
</div>
```

Give the `<div>` a size that matches one of the [supported display ad sizes](https://github.com/GameDistribution/GD-HTML5/wiki/Display-Ads).

You show and hide a Display Ad like this:

```
-- show it
gdsdk.show_display_ad("canvas-ad")

-- hide it
gdsdk.hide_display_ad("canvas-ad")
```


## Verify the SDK implementation
To verify your SDK implementation; make sure to completely view an advertisement, while viewing your game through an iframe launched from the Upload tab of the GameDistribution developer portal page for you game.
