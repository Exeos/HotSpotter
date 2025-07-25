# HotSpotter

## Project Overview

**HotSpotter** is a dynamic reverse engineering tool for the Java Virtual Machine (JVM). The goal is to provide a powerful GUI for attaching to running JVMs, inspecting classes, setting low-level hooks, and patching code? and data at runtime.

---

## Tech Stack

| Layer          | Technology     |
|----------------|----------------|
| Language       | C++            |
| GUI            | Dear ImGui     |
| JVM Interface  | JVMTI, JNI     |
| Build          | CMake          |

---

## Feature Roadmap

### v1: Core Features
- [X] Attach to running JVMs
- [X] Display JVM/process info
- [ ] Enumerate all loaded classes
- [ ] Hook to class load event
- [ ] Class hierarchy navigation
- [ ] View class details: classinfo, fields, methods
- [ ] Dump classes to disk

### v2: Event Hooking
- [ ] Select class/method/field in UI to set specific hooks
- [ ] Set and filter for:  
  - Method Entry events (all/class/method-specific)
  - Native Method Bind (all/class/method-specific)
  - Field Access/Modification (all/class/field-specific)
- [ ] Display live events, filter by selection

### v3: Runtime Patching
- [ ] Select class tab to:
  - View full class info
  - View decompiled output (integrate decompiler)
  - View raw class bytecode
  - List all instances of a class
    - Select instance to view/patch fields
- [ ] Patch fields (value, access modifiers)
- [ ] Patch/replace method bytecode (if technically possible)

---

## Possible Future Enhancements
- [ ] Scripting or Plugins (Python or Lua)
- [ ] Seperate core and frontend, so both can easly be replaced by custom solutions (maybe kernel mode core + web frontend if you want to be fancy)
