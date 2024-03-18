import scrapy
from scrapy import Spider
from scrapy import Request
from scrapy.crawler import CrawlerProcess
from scrapy.linkextractors import LinkExtractor
import os

class FollowAllSpider(Spider):
    name = 'follow_all'
    start_urls = ['https://quotes.toscrape.com/']
    try:
        os.remove('crawlerout.txt')
    except OSError:
        pass

    custom_settings = {
        'CONCURRENT_REQUESTS' : 2,
        'AUTO_THROTTLE_ENABLED' : True,
        'LOG_LEVEL' :  "INFO",
        'RETRY_ENABLED' : False,
    }
    
    def __init__ (self):
        self.link_extractor = LinkExtractor()

    def parse(self, response):
        with open('crawlerout.txt', 'a+') as f:
            f.write(f"{response.url}")
            for link in self.link_extractor.extract_links(response):
                f.write(f"       {str(link.url)}")
            f.write("\n")
            for link in self.link_extractor.extract_links(response):
                if (link.url != response.url):
                    yield response.follow(url=link, callback=self.parse)

if __name__ == "__main__":
    process = CrawlerProcess()
    process.crawl(FollowAllSpider)
    process.start()
