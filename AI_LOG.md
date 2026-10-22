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

