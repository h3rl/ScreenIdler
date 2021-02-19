@echo off
git init
git remote add origin https://github.com/h3rl/ScreenIdler.git
git fetch

setlocal enabledelayedexpansion
set testl=%cmdcmdline:"=%
set testr=!testl:%~nx0=!
if not "%testl%" == "%testr%" pause