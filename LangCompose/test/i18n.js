
const locales = {
fr: () => import("./locales/fr.json"),
en: () => import("./locales/en.json"),
ge: () => import("./locales/ge.json"),
hi: () => import("./locales/hi.json"),
ru: () => import("./locales/ru.json"),
ja: () => import("./locales/ja.json"),
};

let translations = {};
let currentLang = "en";

export async function setLanguage(lang) {
    if (!locales[lang]) {
        lang = "en";
    }

    const module = await locales[lang]();
    translations = module.default;
    currentLang = lang;

    localStorage.setItem("language", lang);
}

export function t(str) {
    return translations[str] ?? str;
}

export function getLanguage() {
    return currentLang;
}
