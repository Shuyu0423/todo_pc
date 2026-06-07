# UI Follow-up Plan

## Reference Direction

Use the second TodoFocus screenshot supplied by the user as the next visual target.

Key traits to preserve:

- Soft glass-like app shell with light blue shadows.
- Left sidebar with clean vector icons and small count pills.
- Dashboard metric cards with lightweight illustrative charts/icons.
- Task detail panel with icon-led metadata rows and a clean checklist.
- Weekly schedule blocks with pastel category colors and tighter typography.
- Floating focus card with transparent glass, rounded corners, and real draggable behavior.

## Icon Plan

The current Qt text-symbol icons are being removed because they can render as missing glyphs or乱码 on some Windows setups.

The app now uses local SVG resources instead of Unicode text icons:

- Prefer SVG icons from Alibaba Iconfont when replacing or adding icons.
- Store downloaded SVG assets under `todo_app/assets/icons/`.
- Keep file names stable, because `todo_app/resources.qrc` packages them by name.
- Render icons through Qt resource paths such as `:/icons/today.svg`.
- Avoid using Unicode symbols for UI icons unless the glyph is guaranteed by the selected font.

## Layout Fix Priorities

1. Replace sidebar text-only navigation with stable icon + text rows.
2. Replace metric-card placeholders with drawn or SVG mini charts.
3. Make the right task detail panel match the reference metadata layout.
4. Continue replacing default Qt controls with custom-painted delegates or styled wrappers.
5. Keep all visible numbers connected to real task, focus, and schedule data.
