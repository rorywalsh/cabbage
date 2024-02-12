import os.path
import os
import re
import glob
import shutil
import xml.etree.ElementTree as ET


currentDir = os.path.abspath(".")

opcodesFile = currentDir+"/opcodes.xml"  

entry = '''
completionTriggerKeywords.push(
{{
        label: '{0}',
        kind: monaco.languages.CompletionItemKind.Function,
        insertText: '{0}',
        description: '{1}',
        insertTextRules: monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
}})'''.format(1,2)

# print(entry)

if(os.path.exists(opcodesFile)):
    tree = ET.parse(opcodesFile)
    root = tree.getroot()
    for opcode in root.iter('opcode'):
        d = ''
        if hasattr(opcode.find('desc'), 'text'):
            d = opcode.find('desc').text.replace('\\', '').replace('\n','').replace('   ', '').replace('\"', '\\"')
        cnt = 0
        o = opcode.findall('synopsis')
        name = ''
        for syn in o:
            if hasattr(syn.find('opcodename'), 'text'):
                name = syn.find('opcodename').text
            if cnt == 0:
                e = "".join(syn.itertext()).replace('\n', '').replace('\\', '')
                e = e[e.index(name)+len(name):]
                entry = '''{{
        label: '{0}',
        kind: monaco.languages.CompletionItemKind.Function,
        insertText: '{0}',
        description: '{1}',
        documentation: "{2}",
        insertTextRules: monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
}},'''.format(name," ".join(e.split()), " ".join(d.split()))
                print(entry)
            cnt+=1

else:
    print("Can't find opcodes.xml")