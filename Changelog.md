# Release notes

## Not released yet

**ADDED** Added option to disable dimming disabled elements using a config

**IMPROVED** 5 configs are optional: `PAGES`, `Orientation`, `MINPRESSURE`, `MAXPRESSURE` and `BACKLIGHT_PIN`  
**IMPROVED** `checkPage()` is more modular

**FIXED** Buttons can't be clicked when invisible  
**FIXED** Sliders can't be changed when invisible

**CHANGED** `changeBrightness()` does not map to 12-bit for DAC pins on Arduino Due  
**CHANGED** removed some unneeded description from `config.ino`

## v0.0.2

**ADDED** Completed Check-box modifier functions  
**ADDED** Slider modifier functions  
**ADDED** FreeSans fonts can be disabled

**IMPROVED** Slider minimum and maximum are stored in memory and aren't required in `getSliderValue()`  
**IMPROVED** Full support for smart screen-wake refresh  
**IMPROVED** Less warnings related to `-fpermissive`  
**IMPROVED** Elements can be added after calling `start()`  
**IMPROVED** A lot more description comments in source code and cleaner code

**FIXED** `calculateListX()` is now `calculateListY()`  
**FIXED** check-boxes can no longer be switched when they're invisible  
**FIXED** `addslider()` now gets value as `int`, which can contain negative numbers

**DEPRECATED** Do not give min & max to `getSliderValue()`, it will become unavailable in next release

## v0.0.1

Initial release
