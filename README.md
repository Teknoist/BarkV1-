# BarkV1 (fork of LARKPlayer)

This repository contains an initial implementation of chapter selector support for the LARKPlayer codebase. It includes backend chapter extraction from M4B (chpl atom) and a GTK dialog to select chapters. This is prepared for the Teknoist/BarkV1 repository as requested.

Features:
- MP4/M4B chpl atom parsing via existing mp4read module
- Chapters vector in the MusicBackend
- GTK dialog to display chapters and seek to a chapter

Build instructions: same as original project (cross-compile for Kindle HF if needed). See original project's README.
