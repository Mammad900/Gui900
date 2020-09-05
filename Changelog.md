# Release notes

## Not released yet

**ADDED** *ELEMENTS* Support for radio buttons  
**ADDED** *ELEMENTS* Support for vertical sliders

## v0.0.3

**ADDED** *CONFIGS* Added option to disable dimming disabled elements using a config

**IMPROVED** *CONFIGS* 5 configs are optional: `PAGES`, `Orientation`, `MINPRESSURE`, `MAXPRESSURE` and `BACKLIGHT_PIN`  
**IMPROVED** *INTERNAL* `checkPage()` is more modular

**FIXED** *BUG* Buttons can't be clicked when invisible  
**FIXED** *BUG* Sliders can't be changed when invisible

**CHANGED** *INTERNAL* `changeBrightness()` does not map to 12-bit for DAC pins on Arduino Due  
**CHANGED** *API* removed some unneeded description from `config.ino`

## v0.0.2

**ADDED** *API* Completed Check-box modifier functions  
**ADDED** *API* Slider modifier functions  
**ADDED** *CONFIGS* FreeSans fonts can be disabled

**IMPROVED** *API* Slider minimum and maximum are stored in memory and aren't required in `getSliderValue()`  
**IMPROVED** *UI* Full support for smart screen-wake refresh  
**IMPROVED** *COMPILE* Less warnings related to `-fpermissive`  
**IMPROVED** *API* Elements can be added after calling `start()`  
**IMPROVED** *INTERNAL* A lot more description comments in source code and cleaner code

**FIXED** *TYPO* `calculateListX()` is now `calculateListY()`  
**FIXED** *BUG* check-boxes can no longer be switched when they're invisible  
**FIXED** *BUG* `addslider()` now gets value as `int`, which can contain negative numbers

**DEPRECATED** *API* Do not give min & max to `getSliderValue()`, it will become unavailable in next release

## v0.0.1

Initial release
