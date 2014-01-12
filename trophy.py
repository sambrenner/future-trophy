from sys import argv
import feedparser
import json
import urllib

trophy_data_path = 'http://www.samjbrenner.com/projects/trophy/trophy.json'
nfl_news_path = ''
nfl_news_intro = ''
news_stories_max = 0
message_buffer_path = 'message_buffer.txt'
offline_data_path = 'offline_data.txt'

# arduino interface functions
def init():
  try:
    get_trophy_data()
  except IOError as e:
    print "No network connection"
    # no internet fallback

def get_next_message():
  message_buffer = open(message_buffer_path, 'a+')
  message_buffer.seek(0)

  # if file is empty, refill buffer
  if message_buffer.readline() == '':
    with open(offline_data_path) as offline_data:
      message_buffer.writelines("%s" % item for item in offline_data.readlines()[1:])
    message_buffer.writelines("%s\n" % item for item in get_nfl_news())
    
  # get first line
  message_buffer.seek(0)
  next_message = message_buffer.readline()
  
  # replace file with all lines except for the first line
  remaining_lines = message_buffer.readlines()
  message_buffer.seek(0)
  message_buffer.truncate()
  message_buffer.writelines("%s" % item for item in remaining_lines)
  message_buffer.close()

  return next_message

# network functionality
def get_nfl_news():
  nfl_feed = feedparser.parse(nfl_news_path)
  
  titles = [nfl_news_intro]

  for entry in nfl_feed.entries[:news_stories_max]:
    titles.append(entry.title)

  return titles

def get_trophy_data():
  global nfl_news_path
  global nfl_news_intro
  global news_stories_max

  json_response = urllib.urlopen(trophy_data_path)
  data = json.loads(json_response.read())

  nfl_news_path = data['nfl_news_path']
  nfl_news_intro = data['nfl_news_intro']
  news_stories_max = data['news_stories_max']

  store_offline_data(data)

# file i/o
def store_offline_data(data):
  offline_data = open(offline_data_path, 'w')

  offline_data.write("%s\n" % data['offline_warning'])

  for line in data['intro_message']:
    offline_data.write("%s\n" % line)

  for champion in data['champions']:
    offline_data.write("%s: %s, %s\n" % (champion['year'], champion['owner'], champion['teamname']))

  offline_data.close()

# instructions
if len(argv) > 1:
  if argv[1] == 'init':
    init()
  elif argv[1] == 'nextmsg':
    get_next_message()
  else:
    print 'Please supply a valid command.'
else:
  init()
  get_next_message()
  'Please supply a command.'