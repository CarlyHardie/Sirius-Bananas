# AI_LOG.md

GenAI use log for Astro 331 Lab 8 (GenAI Level 4). One entry per agent session. The commit history and PR discussion are also part of the record.

## 2026-10-05 · Repo setup and CLAUDE.md
Session:      https://claude.ai/code/session_015DKK6MsrrfSyzUeSCWuuyh
Driver:       <cadet name>
Reviewers:    <cadet names>
Prompts:      /init (draft CLAUDE.md), orientation of Lab 7 code, open PR and merge
Agent made:   CLAUDE.md (commits f84c2e4, e188d5b; PR #2)
We changed:   <what you corrected or added, e.g. hardware facts the agent could not see>
Verified:     <what you checked, e.g. read CLAUDE.md against src/main.cpp and platformio.ini>
Issue found:  CLAUDE.md first said the repo had no code because the session's clone predated the Lab 7 commit. Fixed after fetching.

## <date> · <requirement>
Session:      <claude.ai/code link>
Driver:       <cadet name>
Reviewers:    <cadet names>
Prompts:      <orientation, plan, implement, fixes>
Agent made:   <commits, PR #>
We changed:   <your edits to the agent's work>
Verified:     <hardware test and result>
Issue found:  <anything that failed on the first upload>

## 2026-10-05 · Sun tracking (command 10)
Session:      https://claude.ai/code/session_015DKK6MsrrfSyzUeSCWuuyh
Driver:       <cadet name>
Reviewers:    <cadet names>
Prompts:      requirement, plan (asked for plan before code), answers (INA238 for panel IV, panel faces sun_direction = 180), implement
Agent made:   sun_tracking_test(), read_sun_sensor(), wrap_180(), INA238 setup, command 10, constants in definitions.h (branch claude/clever-hopper-erjpxr)
We changed:   <your edits, e.g. gains, signs, INA238 address/shunt after checking your board>
Verified:     <hardware test and result; the agent could only syntax-check against stubs, no PlatformIO build>
Issue found:  <anything that failed on the first upload>

## 2026-10-22 · Sun tracking: PD to PID
Session:      https://claude.ai/code/session_015DKK6MsrrfSyzUeSCWuuyh
Driver:       <cadet name>
Reviewers:    <cadet names>
Prompts:      explain how a PID controller works, change sun tracking to PID (reason: overshoot seen in hardware test)
Agent made:   integral term with limit, error zone and reset in sun_tracking_test(); TRACK_KI, TRACK_I_LIMIT, TRACK_I_ZONE_DEG in definitions.h; P/I/D columns in the CSV
We changed:   <your gains and any other edits>
Verified:     <hardware test and result; the agent could only syntax-check against stubs>
Issue found:  Hardware test (SunTrack_data004.csv, PD, KI = 0) swung about 90 deg past the target each way and was in tolerance 21% of the time. The log showed the D term had the wrong sign (TRACK_GYRO_SIGN was +1.0), so it pushed with the spin; changed to -1.0. After the change: <what the swing looked like>

## 2026-10-22 · Sun tracking: steady offset and sensor logging
Session:      https://claude.ai/code/session_015DKK6MsrrfSyzUeSCWuuyh
Driver:       <cadet name>
Reviewers:    <cadet names>
Prompts:      shared hardware logs (SunTrack_data007, data009), asked why the satellite stops short and holds off the light
Agent made:   TRACK_KI set to 0.0005; four raw sun sensor readings and S_mag added to the CSV and S: added to the Serial line
We changed:   <your edits>
Verified:     <hardware test and result>
Issue found:  data009 showed a steady 24 deg offset (I term was 0) and the sun reading pinned at exactly 90.0 and 180.0 (one channel lit) while the satellite turned about 74 and 90 deg. After the change: <offset left, and raw channel counts where the reading goes flat>

## 2026-10-22 · Sun tracking: stop at the brightest -X reading
Session:      https://claude.ai/code/session_015DKK6MsrrfSyzUeSCWuuyh
Driver:       <cadet name>
Reviewers:    <cadet names>
Prompts:      shared SunTrack_data013.csv (raw channels), asked what the columns mean, asked for the satellite to stop when the -X reading is at its maximum
Agent made:   APPROACH/SCAN/HOLD modes in sun_tracking_test(), new constants in definitions.h, SUN_MIN_SIGNAL 200 -> 30, five new CSV columns (mode, bright_f, bright_max, angle_since_peak, centered)
We changed:   <your edits, e.g. TRACK_SCAN_PWM, TRACK_MIN_PWM, TRACK_PEAK_DROP>
Verified:     <hardware test and result; the agent could only syntax-check and run a toy simulation>
Issue found:  <what the satellite did: did it scan, pass the peak, come back, and where did it stop?>

## 2026-10-26 · Sun tracking: search when no face sees the light
Session:      https://claude.ai/code/session_015DKK6MsrrfSyzUeSCWuuyh
Driver:       <cadet name>
Reviewers:    <cadet names>
Prompts:      shared SunTrack_data031.csv, asked for the satellite to keep going the way it was until it sees the sun again
Agent made:   search mode (mode 3) in sun_tracking_test(), TRACK_SEARCH_* constants in definitions.h
We changed:   <your edits, e.g. TRACK_SEARCH_PWM>
Verified:     <hardware test and result; the agent could only syntax-check>
Issue found:  In data031 all four channels read 0 for 7.7 s between the +Y and -X faces and the wheel command was 0. After the change: <did it find the -X face, and how long did it take?>

## 2026-10-26 · Sun tracking: search keeps the same wheel command
Session:      https://claude.ai/code/session_015DKK6MsrrfSyzUeSCWuuyh
Driver:       <cadet name>
Reviewers:    <cadet names>
Prompts:      reported it still gets stuck between faces, asked for it to keep going with the same input until a face sees the sun
Agent made:   search now holds the last wheel command from before the light was lost (floor TRACK_SEARCH_PWM, cap TRACK_SEARCH_MAX_PWM), starts after 2 readings (TRACK_LOST_SAMPLES = 2)
We changed:   <your edits>
Verified:     <hardware test and result; the agent could only syntax-check>
Issue found:  <did it find the next face? how long did it take?>

## 2026-10-26 · Sun tracking: stuck between the +Y and -X faces
Session:      https://claude.ai/code/session_015DKK6MsrrfSyzUeSCWuuyh
Driver:       <cadet name>
Reviewers:    <cadet names>
Prompts:      shared SunTrack_data038.csv, asked to analyze it and then to make the three fixes
Agent made:   TRACK_SCAN_PWM and TRACK_SEARCH_PWM 0.15/0.20 -> 0.28; a real-peak rule before SCAN -> HOLD (TRACK_PEAK_MIN_FRACTION, TRACK_PEAK_MIN_RATE_DPS); an I term in HOLD (TRACK_HOLD_KI)
We changed:   <your edits>
Verified:     <hardware test and result; the agent could only syntax-check and replay the rule on logged data>
Issue found:  data038 looped 0 -> 1 -> 2 -> 0 fourteen times in 27 s. Scan (0.15) turned the satellite only ~2 dps, and a 25 count dip at ~300 counts (9% of the brightest face) was taken for the peak. After the change: <did it get through the -X view and stop at the brightest point?>

## 2026-10-28 · Sun tracking simulator (HTML)
Session:      https://claude.ai/code/session_015DKK6MsrrfSyzUeSCWuuyh
Driver:       <cadet name>
Reviewers:    <cadet names>
Prompts:      asked for an HTML simulation of the control system with editable parameters, a visual of the satellite and Sun, and CSV export, from a photo of the setup and a whiteboard sketch
Agent made:   Simulation/SunTrackSim.html (PID of command 10, 4-face sun sensor model, wheel deadband and lag, plots, CSV export, overlay of a test CSV)
We changed:   <your edits, e.g. measured inertias>
Verified:     <compare against a test run; the agent checked the script parses, ran the physics in Node, and looked at one screenshot>
Issue found:  <where the simulation and the test disagree>

## 2026-10-28 · Sun position from opposite-face differences
Session:      https://claude.ai/code/session_015DKK6MsrrfSyzUeSCWuuyh
Driver:       <cadet name>
Reviewers:    <cadet names>
Prompts:      asked to subtract the -X light from +X (and -Y from +Y) so room light does not matter
Agent made:   sun_dX, sun_dY, sun_signal in read_sun_sensor(); "sun visible" now uses sun_signal instead of the four-face total; three new CSV columns; simulator kept in sync
We changed:   <your edits>
Verified:     <hardware test and result; the agent could only syntax-check>
Issue found:  The direction already used the differences (the division by the total does not change atan2). Later changed: sun_X and sun_Y are now the raw differences in counts (not -1...+1) and sun_signal was removed; visibility uses the four-face total again. The angle is still only 0/90/180/270 when one face is lit. After the change: <what did sun_X and sun_Y read at your stopping point?>

## 2026-10-28 · Live sun direction on the ground station
Session:      https://claude.ai/code/session_015DKK6MsrrfSyzUeSCWuuyh
Driver:       <cadet name>
Reviewers:    <cadet names>
Prompts:      asked to show the sun direction live on the KestrelSAT interface instead of the data point number
Agent made:   XBee line "sDir:<deg>,s+X:<counts>,s-X:<counts>,s+Y:<counts>,s-Y:<counts>" 10 times a second (TRACK_XBEE_EVERY) in sun_tracking_test(), mirrored to Serial; replaces the "tp:<n>" counter
We changed:   <your edits>
Verified:     <does the GCS show/plot sDir live? any dropped characters?>
Issue found:  <anything>

## 2026-10-28 · Shorter sun tracking CSV
Session:      https://claude.ai/code/session_015DKK6MsrrfSyzUeSCWuuyh
Driver:       <cadet name>
Reviewers:    <cadet names>
Prompts:      asked whether all CSV columns are needed; the IV requirement was dropped
Agent made:   removed panel_V, panel_I (the INA238 is no longer read during the test), sun_X, sun_Y, in_tolerance and S_mag from the SunTrack CSV and the panel V/I from the Serial line
We changed:   <your edits>
Verified:     <does the CSV have 19 columns and does the simulator overlay still read it?>
Issue found:  <anything>

## 2026-10-28 · Scan, hold and search in the simulator
Session:      https://claude.ai/code/session_015DKK6MsrrfSyzUeSCWuuyh
Driver:       <cadet name>
Reviewers:    <cadet names>
Prompts:      asked to add the scan, hold and search modes to the simulation
Agent made:   ported the firmware control step into Simulation/SunTrackSim.html (same variable names and constants), a mode plot, mode/brightness columns in the CSV export and overlay, optional gyro noise, TRACK_SIGN choice
We changed:   <your edits>
Verified:     <compare the mode plot with a test CSV overlay; the agent ran the logic in Node and looked at one screenshot>
Issue found:  In the simulation, scan/hold/search ends about 5 deg from the lamp from every start angle tried (PID alone: 32 deg). With a very dim lamp the -X face never reaches TRACK_PEAK_MIN_COUNT, scan never starts and it loops between approach and search at the edge of the view.


## 2026-10-30 · Solar panel power back in the sun tracking test
Session:      https://claude.ai/code/session_015DKK6MsrrfSyzUeSCWuuyh
Driver:       <cadet name>
Reviewers:    <cadet names>
Prompts:      asked to show the IV data on KestrelSAT as power (V x I) and add power, I and V columns to the CSV
Agent made:   sun_tracking_test() reads the INA238 every sample (same calls as Lab 3), power = panel_V x panel_I_mA (mW); CSV gets panel_V(V), panel_I(mA), panel_P(mW) at the end (22 columns); live XBee line adds ",P_mW:<power>"
We changed:   <your edits>
Verified:     <does the build pass? does P_mW go up when the panel faces the lamp? is the current positive or negative?>
Issue found:  <anything>
