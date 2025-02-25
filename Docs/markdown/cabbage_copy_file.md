# cabbageCopyFile

This opcode will copy a file, or files, to a new location. This can be useful if you want your plugin to be capable of saving custom packages. The new location, although only a folder, can feature an extension. This opcode runs at i-time only. 

<blockquote style="font-style:italic;border-left:10px solid #93d200;color:rgb(3, 147, 210);padding:1px;padding-left:10px;margin-top:0px;margin-bottom:1px;border-left-width:0.25rem"> Added in Cabbage v2.6.6</blockquote>

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
