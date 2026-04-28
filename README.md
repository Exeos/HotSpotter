# HotSpotter

## Project Overview

**HotSpotter** is a dynamic reverse engineering tool for the Java Virtual Machine (JVM). The goal is to provide a
powerful GUI for attaching to running JVMs, inspecting classes, setting low-level hooks, and patching code? and data at
runtime.

---

## Tech Stack

### Agent
c++, cmake, jni, some ipc

### GUI
Java based gui bcs objectweb asm support

---

## Feature Roadmap

### v1: Core Features

- [ ] Make Gui that can inject the Agent into a JVM
- [ ] Make agent and gui communicate via ipc
- [ ] Attach to running JVMs
- [ ] List loaded classes
- [ ] Navigate classes
- [ ] View bytecode
- [ ] View decompiled bytecode
- [ ] Set breakpoints in bytecode

### v2: Event Hooking

- [ ] Select class/method/field in UI to set specific hooks
- [ ] Set and filter for:
    - Method Entry events (all/class/method-specific)
    - Native Method Bind (all/class/method-specific)
    - Field Access/Modification (all/class/field-specific)
- [ ] Display live events, filter by selection

### v3: Runtime Patching

- [ ] Patch fields (value, access modifiers)
- [ ] Patch/replace method bytecode
- [ ] Scripting or Plugins (Python or Lua)

---
