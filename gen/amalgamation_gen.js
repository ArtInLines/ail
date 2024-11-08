const path = require('path');
const fs   = require('fs');

const dir_path = path.join(__dirname, "..");
const deprecated = ["ail_gui.h"]
const amalgamated_name = "ail_all.h";

let all = fs.readdirSync(dir_path, { withFileTypes: true })
    .filter(dirent => dirent.isFile() && dirent.name != amalgamated_name && !deprecated.includes(dirent.name) && dirent.name.startsWith("ail") && dirent.name.endsWith(".h"))
    .map(dirent => dirent.name)
    .sort()
    .map(filename => {
        let content = fs.readFileSync(path.join(dir_path, filename), { encoding: "utf8" });
        content = `////////\n// From ${filename}\n////////\n\n\n` + content;
        return content.replace(/#include\s+"ail.*\.h"\n/, "");
    })
    .join("\n\n\n\n\n");

fs.writeFileSync(path.join(dir_path, amalgamated_name), all);
