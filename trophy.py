from sys import argv
import feedparser
import json
import urllib

trophy_data_path = 'http://www.samjbrenner.com/projects/trophy/trophy.json'
nfl_news_path = ''
news_stories_max = 0
message_buffer_path = 'message_buffer.txt'
offline_data_path = 'offline_data.txt'

# arduino interface functions
def init():
  get_trophy_data()
  get_nfl_news()

def get_next_message():
  print 'Test'

# network functionality
def get_nfl_news():
  nfl_feed = feedparser.parse(nfl_news_path)
  for entry in nfl_feed.entries:
    print entry.title

def get_trophy_data():
  json_response = urllib.urlopen(trophy_data_path)
  data = json.loads(json_response.read())

  nfl_news_path = data['nfl_news_path']
  news_stories_max = data['news_stories_max']

  store_offline_data(data)

def store_offline_data(data):
  offline_data = open(offline_data_path, 'w')

  offline_data.write(data['offline_warning'])

  for line in data['intro_message']:
    offline_data.write("%s\n" % line)

  for champion in data['champions']:
    offline_data.write("%s: %s, %s\n" % (champion['year'], champion['owner'], champion['teamname']))

  offline_data.close()

if argv[1] == 'init':
  init()
elif argv[1] == 'nextmsg':
  get_next_message()
else:
  print 'Please supply a valid command.'