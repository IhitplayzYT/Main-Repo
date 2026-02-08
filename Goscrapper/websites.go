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

var website_list []string = []string{
	"https://www.google.com",
	"https://www.instagram.com",
	"https://www.twitter.com",
	"https://www.microsoft.com",
	"https://www.linkedin.com",
	"https://www.apple.com",
	"https://www.github.com",
	"https://www.wordpress.org",
	"https://www.youtu.be",
	"https://www.en.wikipedia.org",
	"https://www.play.google.com",
	"https://www.goo.gl",
	"https://www.pinterest.com",
	"https://www.maps.google.com",
	"https://www.x.com",
	"https://www.vimeo.com",
	"https://www.docs.google.com",
	"https://www.apps.apple.com",
	"https://www.whatsapp.com",
	"https://www.tiktok.com",
	"https://www.wa.me",
	"https://www.nginx.org",
	"https://www.f5.com",
	"https://www.nginx.com",
	"https://www.policies.google.com",
	"https://www.bit.ly",
	"https://www.adobe.com",
	"https://www.wordpress.com",
	"https://www.drive.google.com",
	"https://www.api.whatsapp.com",
	"https://www.mozilla.org",
	"https://www.player.vimeo.com",
	"https://www.plus.google.com",
	"https://www.europa.eu",
	"https://www.apache.org",
	"https://www.maps.app.goo.gl",
	"https://www.t.me",
	"https://www.github.io",
	"https://www.itunes.apple.com",
	"https://www.gravatar.com",
	"https://www.yahoo.com",
	"https://www.reddit.com",
	"https://www.forms.gle",
	"https://www.googletagmanager.com",
	"https://www.facebook.com",
	"https://www.youtube.com",
	"https://www.wikipedia.org",
	"https://www.amazon.com",
	"https://www.blogspot.com",
	"https://www.https://www.instagram.com/",
	"https://www.snapchat.com",
	"https://www.tumblr.com",
	"https://www.vk.com",
	"https://www.weibo.com",
	"https://www.bit.ly",
	"https://www.threads.net", "https://www.mastodon.social", "https://www.discord.com", "https://www.telegram.org", "https://www.whatsapp.com",
	"https://www.apache.org", "https://www.maps.app.goo.gl", "https://www.t.me", "https://www.yahoo.com", "https://www.forms.gle", "https://www.googletagmanager.com", "https://www.amazon.com", "https://www.blogspot.com",
	"https://www.kuaishou.com", "https://www.line.me", "https://www.qq.com", "https://www.wechat.com", "https://www.clubhouse.com", "https://www.flickr.com",
	"https://www.github.com", "https://www.gitlab.com", "https://www.bitbucket.org", "https://www.stackoverflow.com", "https://www.stackexchange.com",
	"https://www.medium.com", "https://www.dev.to", "https://www.hashnode.com", "https://www.substack.com", "https://www.behance.net", "https://www.dribbble.com",
	"https://www.about.me", "https://www.angel.co", "https://www.wellfound.com", "https://www.producthunt.com", "https://www.indiehackers.com",
	"https://www.researchgate.net", "https://www.academia.edu", "https://www.orcid.org", "https://www.slideshare.net", "https://www.kaggle.com",
	"https://www.wordpress.com", "https://www.blogspot.com", "https://www.ghost.io", "https://www.wix.com", "https://www.weebly.com",
	"https://www.webflow.io", "https://www.livejournal.com", "https://www.typepad.com", "https://www.hubpages.com", "https://www.squarespace.com",
	"https://www.youtube.com", "https://www.vimeo.com", "https://www.dailymotion.com", "https://www.twitch.tv", "https://www.rumble.com",
	"https://www.odysee.com", "https://www.bilibili.com", "https://www.kick.com",
	"https://www.quora.com", "https://www.4chan.org", "https://www.8kun.top", "https://www.hackernews.com", "https://www.news.ycombinator.com",
	"https://www.slashdot.org", "https://www.somethingawful.com", "https://www.resetera.com", "https://www.neogaf.com",
	"https://www.metafilter.com", "https://www.blueskyweb.xyz",
	"https://www.steamcommunity.com", "https://www.epicgames.com", "https://www.roblox.com", "https://www.minecraft.net",
	"https://www.xbox.com", "https://www.playstation.com", "https://www.riotgames.com", "https://www.battle.net", "https://www.nexusmods.com", "https://www.moddb.com",
	"https://www.soundcloud.com", "https://www.bandcamp.com", "https://www.last.fm", "https://www.reverbnation.com", "https://www.audiomack.com",
	"https://www.mixcloud.com", "https://www.patreon.com", "https://www.buymeacoffee.com", "https://www.ko-fi.com",
	"https://www.amazon.com", "https://www.ebay.com", "https://www.etsy.com", "https://www.aliexpress.com", "https://www.shopee.com",
	"https://www.lazada.com", "https://www.mercari.com", "https://www.poshmark.com", "https://www.grailed.com", "https://www.depop.com",
	"https://www.crunchbase.com", "https://www.pitchbook.com", "https://www.f6s.com", "https://www.betalist.com", "https://www.ycombinator.com", "https://www.gust.com",
	"https://www.tripadvisor.com", "https://www.booking.com", "https://www.airbnb.com", "https://www.expedia.com", "https://www.yelp.com",
	"https://www.zomato.com", "https://www.swarmapp.com", "https://www.foursquare.com",
	"https://www.wikipedia.org", "https://www.wikidata.org", "https://www.openstreetmap.org", "https://www.geonames.org",
	"https://www.opencorporates.com", "https://www.sec.gov", "https://www.companieshouse.gov.uk", "https://www.data.gov", "https://www.data.gov.in", "https://www.worldbank.org", "https://www.un.org", "https://www.cia.gov",
	"https://www.crt.sh", "https://www.who.is", "https://www.viewdns.info", "https://www.dnsdumpster.com", "https://www.builtwith.com",
	"https://www.securitytrails.com", "https://www.ipinfo.io", "https://www.bgp.he.net", "https://www.shodan.io", "https://www.censys.io",
	"https://www.bbc.com", "https://www.reuters.com", "https://www.apnews.com", "https://www.aljazeera.com", "https://www.theguardian.com",
	"https://www.nytimes.com", "https://www.washingtonpost.com", "https://www.forbes.com", "https://www.bloomberg.com",
	"https://www.economist.com", "https://www.ft.com", "https://www.wsj.com", "https://www.cnn.com", "https://www.nbcnews.com",
	"https://www.cbsnews.com", "https://www.abcnews.go.com", "https://www.foxnews.com", "https://www.ndtv.com",
	"https://www.timesofindia.indiatimes.com", "https://www.hindustantimes.com", "https://www.thehindu.com", "https://www.indianexpress.com",
	"https://www.coursera.org", "https://www.edx.org", "https://www.udemy.com", "https://www.udacity.com", "https://www.khanacademy.org",
	"https://www.unsplash.com", "https://www.pexels.com", "https://www.pixabay.com", "https://www.500px.com", "https://www.deviantart.com", "https://www.artstation.com",
	"https://www.npmjs.com", "https://www.pypi.org", "https://www.rubygems.org", "https://www.crates.io", "https://www.cpan.org", "https://www.packagist.org",
	"https://www.strava.com", "https://www.fitbit.com", "https://www.garmin.com", "https://www.myfitnesspal.com",
	"https://www.duolingo.com", "https://www.italki.com", "https://www.hellotalk.com",
	"https://www.okcupid.com", "https://www.bumble.com", "https://www.hinge.co", "https://www.match.com", "https://www.tinder.com",
	"https://www.mixi.jp", "https://www.renren.com", "https://www.douban.com", "https://www.skyrock.com", "https://www.xing.com", "https://www.viadeo.com",
	"https://www.superuser.com", "https://www.serverfault.com", "https://www.askubuntu.com", "https://www.mathoverflow.net",
	"https://www.letterboxd.com", "https://www.goodreads.com", "https://www.imdb.com", "https://www.rottentomatoes.com",
	"https://www.pornhub.com", "https://www.xvideos.com", "https://www.xnxx.com", "https://www.xhamster.com", "https://www.redtube.com",
	"https://www.youporn.com", "https://www.spankbang.com", "https://www.tube8.com", "https://www.tnaflix.com", "https://www.beeg.com",
	"https://www.manyvids.com", "https://www.chaturbate.com", "https://www.stripchat.com", "https://www.camsoda.com", "https://www.bongacams.com",
	"https://www.myfreecams.com", "https://www.cam4.com", "https://www.flirt4free.com", "https://www.livejasmin.com", "https://www.imlive.com",
	"https://www.onlyfans.com", "https://www.fansly.com", "https://www.justforfans.com", "https://www.fanhouse.app",
	"https://www.modelhub.com", "https://www.iwantclips.com", "https://www.clips4sale.com", "https://www.manyclips.com",
	"https://www.adultwork.com", "https://www.escortdirectory.com", "https://www.eros.com", "https://www.tryst.link",
	"https://www.fetlife.com", "https://www.alt.com", "https://www.adultfriendfinder.com", "https://www.ashleymadison.com",
	"https://www.literotica.com", "https://www.sexstories.com", "https://www.lushstories.com",
	"https://www.hentai-foundry.com", "https://www.rule34.xxx", "https://www.gelbooru.com", "https://www.danbooru.donmai.us",
	"https://www.e621.net", "https://www.furaffinity.net", "https://www.newgrounds.com", "https://www.hentaifox.com",
	"https://www.nhentai.net", "https://www.exhentai.org", "https://www.hentaigasm.com",
}
