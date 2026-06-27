import { setLanguage, t, getLanguage} from "../i18n.js";
let name = t("ihit");
console.log(t("hello"));
console.log(t("hello %s"), ihit);
console.log(t("`hello ${name}`")
    .replace("${name}", name))
