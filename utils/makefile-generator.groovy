def files = [:]
new File("./src").eachFileMatch(~/.*\.c/) { file ->
    files.put "src/$file.name", file.name.replace('.c','.o')
}
new File("./src/lib").eachFileMatch(~/.*\.c/) { file ->
    files.put "src/lib/$file.name", file.name.replace('.c','.o')
}

def templateText = new File("./utils/makefile-template.txt").text

def compileFinder =
templateText = templateText.replace("___link_objects___", files.keySet().collect { key ->
  return "\t\"\$(OUTPUT)/${files.get(key)}\""
}.join(" \\\n"))

templateText = templateText.replace("___ouput_objects___", files.keySet().collect { key ->
  return "\t\$(OUTPUT)/${files.get(key)}"
}.join(" \\\n"))

templateText = templateText.replace("___compile_objects___", files.keySet().collect { key ->
  return """\$(OUTPUT)/${files.get(key)}: ./$key
\t\t\$(CC) \$(CFLAGS) -ObjN=\"\$@\" -Lm=\"\$@.d\" -LmCfg=xilmou \$<"""
}.join("\n\n"))

new File("Makefile").text = templateText
