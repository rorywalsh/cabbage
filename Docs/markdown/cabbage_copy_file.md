# cabbageCopyFile

This opcode will copy a file, or files, to a new location. This can be useful if you want your plugin to be capable of saving custom packages. The new location, although only a folder, can feature an extension. This opcode runs at i-time only. 

### Syntax

<pre><b>cabbageCopyFile</b> SLocation, SFile1 [, SFile2, SFile3, ...]</pre>

#### Initialization

* `SLocation` -- a target location for your copied files
* `SFile1, SFile2, ...` -- the files to be copied. At least one file needs to be provided.  

### Example

```csharp
instr 1
    cabbageCopyFile "../package.pkz", "Sample1.wav", "Sample2.wav", "preset.json", "readme.md"
endin
```

> Added in Cabbage v2.6.6