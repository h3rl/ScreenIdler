@echo off

git fetch origin && git reset --hard origin/main && git clean -f -d


setlocal enabledelayedexpansion
set testl=%cmdcmdline:"=%
set testr=!testl:%~nx0=!
if not "%testl%" == "%testr%" pause