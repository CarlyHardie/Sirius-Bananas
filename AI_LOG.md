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

