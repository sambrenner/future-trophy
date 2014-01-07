import feedparser
import json
import urllib

message_buffer_path = 'message_buffer.txt'
nfl_news_path = 'http://www.cbssports.com/partners/feeds/rss/nfl_news'
champions_path = 'http://www.samjbrenner.com/projects/trophy/champions.json'

# functions for arduino to call
def init():
  message_buffer = open(message_buffer_path, 'w+')
  message_buffer.writelines([
    "WESTFIELD ATHLETIC CLUB\n", 
    "TROPHY OF CHAMPIONS\n"
  ])

#def get_next_message():

# network functionality
def get_nfl_news():
  nfl_feed = feedparser.parse(nfl_news_path)
  for entry in nfl_feed.entries:
    print entry.title

def get_champions():
  json_response = urllib.urlopen(champions_path)
  data = json.loads(json_response.read())
  for champion in data['champions']:
    print "%s: %s, %s" % (champion['year'], champion['owner'], champion['teamname'])


get_champions()