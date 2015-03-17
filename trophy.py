from sys import argv
import os
import feedparser
import json
import urllib
import apikey

trophy_data_path = 'http://www.samjbrenner.com/projects/trophy/trophy.json'
nfl_news_path = ''
nfl_news_intro = ''
league_api_path = ''
league_standings_intro = ''
league_api_collection_names = []
news_stories_max = 0
message_buffer_path = 'message_buffer.txt'
offline_data_path = 'offline_data.txt'

def clear_buffer():
  open(message_buffer_path, 'w').close()

def fill_buffer():
  message_buffer = open(message_buffer_path, 'a+')
  message_buffer.seek(0)

  try:
    get_trophy_data()

    with open(offline_data_path) as offline_data:
      message_buffer.writelines("%s" % item for item in offline_data.readlines()[1:])

    message_buffer.writelines("%s\n" % item for item in get_league_data())
    message_buffer.writelines("%s\n" % item for item in get_nfl_news())
    
  except IOError as e:
    with open(offline_data_path) as offline_data:
      message_buffer.writelines("%s" % item for item in offline_data.readlines()[1:])
  
def get_next_message():
  message_buffer = open(message_buffer_path, 'a+')
  message_buffer.seek(0)

  current_lines = message_buffer.readlines()

  if len(current_lines) == 0:
    # if this is the last message, STOP
    next_message = "STOP"
  else:
    # get first line
    message_buffer.seek(0)
    next_message = message_buffer.readline()

  # replace file with all lines except for the first line
  remaining_lines = message_buffer.readlines()
  message_buffer.seek(0)
  message_buffer.truncate()
  message_buffer.writelines("%s" % item for item in remaining_lines)
  message_buffer.close()

  print next_message
  return next_message

# network functionality
def get_trophy_data():
  global nfl_news_path, nfl_news_intro, news_stories_max, league_api_path, league_api_collection_names, league_standings_intro

  json_response = urllib.urlopen(trophy_data_path)
  data = json.loads(json_response.read())

  nfl_news_path = data['nfl_news_path']
  nfl_news_intro = data['nfl_news_intro']
  news_stories_max = data['news_stories_max']
  league_api_path = data['league_api_path'].replace('API_KEY', apikey.value)
  league_api_collection_names = data['league_api_collection_names']
  league_standings_intro = data['league_standings_intro']

  store_offline_data(data)

def get_nfl_news():
  nfl_feed = feedparser.parse(nfl_news_path)
  
  titles = [nfl_news_intro]

  for entry in nfl_feed.entries[:news_stories_max]:
    titles.append(entry.title)

  return titles

def get_league_data():
  json_response = urllib.urlopen(league_api_path)
  data = json.loads(json_response.read())

  standings = [league_standings_intro]

  for collection_name in league_api_collection_names:
    data = data[collection_name]

  for player in sorted(data, key=lambda x:int(x['rank'])):
    standings.append("%s: %s, %s (%s)" % (player['rank'], player['name']['text'], player['record'], player['streak']))

  return standings

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
  if argv[1] == 'nextmsg':
    get_next_message()
  elif argv[1] == 'clearbuffer':
    clear_buffer()
    print "ok"
  elif argv[1] == 'fillbuffer':
    fill_buffer()
    print "ok"
  else:
    print 'Please supply a valid command.'
else:
  print 'Please supply a command.'
