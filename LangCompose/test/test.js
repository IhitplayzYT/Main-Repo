import { setLanguage, t, getLanguage} from "./i18n.js";
let name = t("ihit");
let age = 18
console.log(t("bye"));
console.log(t("bye %s"), name);
console.log(t("`bye ${name} ${age}`")
    .replace("${name}", name)
    .replace("${age}", age))

