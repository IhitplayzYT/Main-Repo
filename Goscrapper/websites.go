package main

import (
	"encoding/csv"
	"os"
)

func boost_wlist() {
	l := len(website_list)
	for i := 0; i < l; i++ {
		website_list = append(website_list, (website_list[i] + "/search?q=" + KEYWORD))
		website_list = append(website_list, (website_list[i] + "/search?query=" + KEYWORD))
		website_list = append(website_list, (website_list[i] + "/?s=" + KEYWORD))
		website_list = append(website_list, (website_list[i] + "/search?keyword=" + KEYWORD))
	}
	website_list = website_list[l+1:]
}

func init_wlist() {
	f, err := os.Open("majestic_million.csv")
	if err != nil {
		panic(err)
	}
	defer f.Close()
	reader := csv.NewReader(f)
	header, err := reader.Read()
	if err != nil {
		panic(err)
	}
	domainIdx := -1
	for i, h := range header {
		if h == "Domain" {
			domainIdx = i
			break
		}
	}

	if domainIdx == -1 {
		panic("Column \"Domain\" not found")
	}
	for {
		row, err := reader.Read()
		if err != nil {
			break
		}
		if domainIdx < len(row) && len(row[domainIdx]) != 0 {
			website_list = append(website_list, row[domainIdx])
		}
	}

}

var website_list = []string{
	"https://duolingo.com", "https://italki.com", "https://hellotalk.com",

	"https://okcupid.com", "https://bumble.com", "https://hinge.co", "https://match.com", "https://tinder.com",

	"https://mixi.jp", "https://renren.com", "https://douban.com", "https://skyrock.com", "https://xing.com", "https://viadeo.com",

	"https://superuser.com", "https://serverfault.com", "https://askubuntu.com", "https://mathoverflow.net",

	"https://letterboxd.com", "https://goodreads.com", "https://imdb.com", "https://rottentomatoes.com",

	"https://pornhub.com", "https://xvideos.com", "https://xnxx.com", "https://xhamster.com", "https://redtube.com",
	"https://youporn.com", "https://spankbang.com", "https://tube8.com", "https://tnaflix.com", "https://beeg.com",
	"https://manyvids.com", "https://chaturbate.com", "https://stripchat.com", "https://camsoda.com", "https://bongacams.com",
	"https://myfreecams.com", "https://cam4.com", "https://flirt4free.com", "https://livejasmin.com", "https://imlive.com",
	"https://onlyfans.com", "https://fansly.com", "https://justforfans.com", "https://fanhouse.app",
	"https://modelhub.com", "https://iwantclips.com", "https://clips4sale.com", "https://manyclips.com",
	"https://adultwork.com", "https://escortdirectory.com", "https://eros.com", "https://tryst.link",
	"https://fetlife.com", "https://alt.com", "https://adultfriendfinder.com", "https://ashleymadison.com",
	"https://literotica.com", "https://sexstories.com", "https://lushstories.com",
	"https://hentai-foundry.com", "https://rule34.xxx", "https://gelbooru.com", "https://danbooru.donmai.us",
	"https://e621.net", "https://furaffinity.net", "https://newgrounds.com", "https://hentaifox.com",
	"https://nhentai.net", "https://exhentai.org", "https://hentaigasm.com",

	"https://google.com", "https://instagram.com", "https://twitter.com", "https://x.com",
	"https://microsoft.com", "https://linkedin.com", "https://apple.com", "https://github.com",
	"https://wordpress.org", "https://pinterest.com", "https://tiktok.com", "https://facebook.com",
	"https://youtube.com", "https://reddit.com", "https://snapchat.com", "https://tumblr.com",
	"https://vk.com", "https://weibo.com", "https://threads.net", "https://mastodon.social",
	"https://discord.com", "https://telegram.org",

	"https://youtu.be", "https://vimeo.com", "https://player.vimeo.com",

	"https://wikipedia.org", "https://en.wikipedia.org", "https://wikidata.org",
	"https://openstreetmap.org", "https://geonames.org",

	"https://play.google.com", "https://maps.google.com", "https://docs.google.com",
	"https://drive.google.com", "https://policies.google.com", "https://forms.gle",
	"https://goo.gl", "https://bit.ly",

	"https://whatsapp.com", "https://wa.me", "https://api.whatsapp.com",

	"https://nginx.org", "https://nginx.com", "https://f5.com", "https://mozilla.org",
	"https://apache.org", "https://adobe.com", "https://europa.eu",

	"https://gitlab.com", "https://bitbucket.org", "https://stackoverflow.com",
	"https://stackexchange.com", "https://news.ycombinator.com", "https://dev.to",
	"https://medium.com", "https://hashnode.com", "https://substack.com",

	"https://researchgate.net", "https://academia.edu", "https://orcid.org", "https://slideshare.net",
	"https://kaggle.com",

	"https://ghost.io", "https://wix.com", "https://weebly.com", "https://webflow.com",
	"https://squarespace.com", "https://livejournal.com", "https://typepad.com",

	"https://dailymotion.com", "https://twitch.tv", "https://rumble.com",
	"https://odysee.com", "https://bilibili.com", "https://kick.com",

	"https://quora.com", "https://4chan.org", "https://8kun.top",
	"https://slashdot.org", "https://metafilter.com",

	"https://steamcommunity.com", "https://epicgames.com", "https://roblox.com", "https://minecraft.net",
	"https://xbox.com", "https://playstation.com", "https://riotgames.com", "https://battle.net",
	"https://nexusmods.com", "https://moddb.com",

	"https://soundcloud.com", "https://bandcamp.com", "https://last.fm",
	"https://reverbnation.com", "https://audiomack.com", "https://mixcloud.com",
	"https://patreon.com", "https://buymeacoffee.com", "https://ko-fi.com",

	"https://amazon.com", "https://ebay.com", "https://etsy.com", "https://aliexpress.com",
	"https://shopee.com", "https://lazada.com", "https://mercari.com", "https://poshmark.com",
	"https://grailed.com", "https://depop.com",

	"https://tripadvisor.com", "https://booking.com", "https://airbnb.com",
	"https://expedia.com", "https://yelp.com", "https://zomato.com",

	"https://sec.gov", "https://companieshouse.gov.uk", "https://data.gov",
	"https://data.gov.in", "https://worldbank.org", "https://un.org", "https://cia.gov",

	"https://crt.sh", "https://who.is", "https://viewdns.info", "https://dnsdumpster.com",
	"https://builtwith.com", "https://securitytrails.com", "https://ipinfo.io",
	"https://bgp.he.net", "https://shodan.io", "https://censys.io",

	"https://bbc.com", "https://reuters.com", "https://apnews.com", "https://aljazeera.com",
	"https://theguardian.com", "https://nytimes.com", "https://washingtonpost.com",
	"https://forbes.com", "https://bloomberg.com", "https://economist.com",
	"https://ft.com", "https://wsj.com", "https://cnn.com", "https://nbcnews.com",
	"https://cbsnews.com", "https://abcnews.go.com", "https://foxnews.com",
	"https://ndtv.com", "https://timesofindia.indiatimes.com",
	"https://hindustantimes.com", "https://thehindu.com", "https://indianexpress.com",

	"https://coursera.org", "https://edx.org", "https://udemy.com", "https://udacity.com", "https://khanacademy.org",

	"https://unsplash.com", "https://pexels.com", "https://pixabay.com", "https://500px.com",
	"https://deviantart.com", "https://artstation.com",

	"https://npmjs.com", "https://pypi.org", "https://rubygems.org", "https://crates.io",
	"https://cpan.org", "https://packagist.org",

	"https://strava.com", "https://fitbit.com", "https://garmin.com", "https://myfitnesspal.com",
}
