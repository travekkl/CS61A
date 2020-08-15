"""Typing test implementation"""

from utils import *
from ucb import main, interact, trace
from datetime import datetime


###########
# Phase 1 #
###########


def choose(paragraphs, select, k):
    """Return the Kth paragraph from PARAGRAPHS for which SELECT called on the
    paragraph returns true. If there are fewer than K such paragraphs, return
    the empty string.
    """
    # BEGIN PROBLEM 1
    # 选择第k个为真的语句
    "*** YOUR CODE HERE ***"
    for i in range(len(paragraphs)):
        if select(paragraphs[i]):
            if k == 0:
                return paragraphs[i]
            k -= 1
    return ''
    # END PROBLEM 1


def about(topic):
    """Return a select function that returns whether a paragraph contains one
    of the words in TOPIC.

    >>> about_dogs = about(['dog', 'dogs', 'pup', 'puppy'])
    >>> choose(['Cute Dog!', 'That is a cat.', 'Nice pup!'], about_dogs, 0)
    'Cute Dog!'
    >>> choose(['Cute Dog!', 'That is a cat.', 'Nice pup.'], about_dogs, 1)
    'Nice pup.'
    """
    assert all([lower(x) == x for x in topic]), 'topics should be lowercase.'
    # BEGIN PROBLEM 2
    # 对于函数select_function，若para中含有topic中的单词，则返回True，否则False
    "*** YOUR CODE HERE ***"
    
    def select_function(para):
        for i in split(remove_punctuation(lower(str(para)))):
            if i in topic:
                return True
        return False
    return select_function
    # END PROBLEM 2


def accuracy(typed, reference):
    """Return the accuracy (percentage of words typed correctly) of TYPED
    when compared to the prefix of REFERENCE that was typed.

    >>> accuracy('Cute Dog!', 'Cute Dog.')
    50.0
    >>> accuracy('A Cute Dog!', 'Cute Dog.')
    0.0
    >>> accuracy('cute Dog.', 'Cute Dog.')
    50.0
    >>> accuracy('Cute Dog. I say!', 'Cute Dog.')
    50.0
    >>> accuracy('Cute', 'Cute Dog.')
    100.0
    >>> accuracy('', 'Cute Dog.')
    0.0
    """
    typed_words = split(typed)

    
    reference_words = split(reference)
    # BEGIN PROBLEM 3
    "*** YOUR CODE HERE ***"
    min = 0
    max = 0
    num = 0
    if len(typed_words) <= len(reference_words):
        min = len(typed_words)
        max = len(reference_words)
        num = min
    else:
        max = len(typed_words)
        min = len(reference_words)
        num = max
    
    if min == 0:
        return 0.0
    count = 0
              
    for i in range(min):
        if typed_words[i] == reference_words[i]:
            count += 1

    return round(count / num * 100, 2)
        
    # END PROBLEM 3


def wpm(typed, elapsed):
    """Return the words-per-minute (WPM) of the TYPED string."""
    assert elapsed > 0, 'Elapsed time must be positive'
    # BEGIN PROBLEM 4
    "*** YOUR CODE HERE ***"
    return len(typed) / 5 * 60 / elapsed
    # END PROBLEM 4


def autocorrect(user_word, valid_words, diff_function, limit):
    """Returns the element of VALID_WORDS that has the smallest difference
    from USER_WORD. Instead returns USER_WORD if that difference is greater
    than LIMIT.
    """
    # BEGIN PROBLEM 5
    "*** YOUR CODE HERE ***"
    if user_word in valid_words:
        return user_word

    min_diff = limit + 1
    cur_diff = 0
    similar_word = user_word
    for word in valid_words:
        cur_diff = diff_function(user_word, word, limit)
        if cur_diff < min_diff:
            min_diff = cur_diff
            similar_word = word

    if min_diff > limit:
        return user_word
    else:
        return similar_word
    # END PROBLEM 5


def swap_diff(start, goal, limit):
    """A diff function for autocorrect that determines how many letters
    in START need to be substituted to create GOAL, then adds the difference in
    their lengths.
    """
    # BEGIN PROBLEM 6
    # assert False, 'Remove this line'
    
    def diff_apply(list_start, list_goal, change_num):
        if list_start == [] and list_goal == []:
            if change_num > limit:
                return limit + 1
            return change_num
        elif list_start == [] and list_goal != []:
            change_num += len(list_goal)
            if change_num > limit:
                return limit + 1
            return change_num
        elif list_start != [] and list_goal == []:
            change_num += len(list_start)
            if change_num > limit:
                return limit + 1
            return change_num
        else:
            if list_start.pop(0) != list_goal.pop(0):
                change_num += 1
            return diff_apply(list_start, list_goal, change_num)

    return diff_apply(list(start), list(goal), 0)



    
    # END PROBLEM 6

def edit_diff(start, goal, limit):
    """A diff function that computes the edit distance from START to GOAL."""
    if limit < 0: # Fill in the condition
        # BEGIN
        return 1
        # END

    elif start == goal: # Feel free to remove or add additional cases
        # BEGIN
        return 0
        # END

    elif len(start) > len(goal) and start[:len(goal)] == goal:
        return len(start) - len(goal)

    elif len(goal) > len(start) and goal[:len(start)] == start:
        return len(goal) - len(start)

    elif start[0] == goal[0]:
        return edit_diff(start[1:], goal[1:], limit)

    else:
        # BEGIN
        return min(1 + edit_diff(start, goal[1:], limit-1), #adding
            1 + edit_diff(start[1:], goal, limit-1), #removing
            1 + edit_diff(start[1:], goal[1:], limit-1)) #substituting
        # END


def final_diff(start, goal, limit):
    """A diff function. If you implement this function, it will be used."""
    assert False, 'Remove this line to use your final_diff function'




###########
# Phase 3 #
###########


def report_progress(typed, prompt, id, send):
    """Send a report of your id and progress so far to the multiplayer server."""
    # BEGIN PROBLEM 8
    "*** YOUR CODE HERE ***"
    right_word = 0
    progress = 0
    for i in range(0, len(typed)):
        if typed[i] == prompt[i]:
            right_word += 1
        else:
            break
    progress = right_word / len(prompt)
    dict_out = {"id":id, "progress":progress}
    send(dict_out)
    return progress
    
    # END PROBLEM 8


def fastest_words_report(word_times):
    """Return a text description of the fastest words typed by each player."""
    fastest = fastest_words(word_times)
    report = ''
    for i in range(len(fastest)):
        words = ','.join(fastest[i])
        report += 'Player {} typed these fastest: {}\n'.format(i + 1, words)
    return report



# 先找出每个单词最快的时间，再用玩家所花的时间去对比，若在边界之内，则保留该单词
def fastest_words(word_times, margin=1e-5):
    """A list of which words each player typed fastest."""
    n_players = len(word_times)#
    n_words = len(word_times[0]) - 1
    assert all(len(times) == n_words + 1 for times in word_times)
    assert margin > 0
    # BEGIN PROBLEM 9
    "*** YOUR CODE HERE ***"

    # Calculate the time per word for each player
    # 计算每个玩家打出每一个单词所用的时间，放在列表all_times里
    all_times = [[] for temp in range(n_players)]
    for player in range(n_players):
        player_wts, times = word_times[player], []
        for wrd in range(n_words):
            times += [elapsed_time(player_wts[wrd+1]) - elapsed_time(player_wts[wrd])]
        all_times[player] = times
    # debug with print(all_times) 

    # Find the fastest time for each word
    # 对于每一个单词，找到最快的时间，放在列表fastest_times
    fastest_times = [None] * len(all_times[0])
    for fw_index in range(len(fastest_times)):
        fastest = all_times[0][fw_index]
        for times in all_times:
            fastest = min(times[fw_index], fastest)
        fastest_times[fw_index] = fastest
    # debug with print(fastest_times) 

    # Create a list of fastest words (within the margin) for each player
    all_words = [[] for temp in range(n_players)]
    for player in range(n_players):
        player_times = all_times[player]
        for wrd in range(n_words):
            if player_times[wrd] - fastest_times[wrd] <= margin:
                all_words[player] += [word(word_times[player][wrd+1])]
    # debug with print(all_words) 

    return all_words
    # END PROBLEM 9
    # END PROBLEM 9


def word_time(word, elapsed_time):
    """A data abstrction for the elapsed time that a player finished a word."""
    return [word, elapsed_time]


def word(word_time):
    """An accessor function for the word of a word_time."""
    return word_time[0]


def elapsed_time(word_time):
    """An accessor function for the elapsed time of a word_time."""
    return word_time[1]


enable_multiplayer = False  # Change to True when you


##########################
# Command Line Interface #
##########################


def run_typing_test(topics):
    """Measure typing speed and accuracy on the command line."""
    paragraphs = lines_from_file('data/sample_paragraphs.txt')
    select = lambda p: True
    if topics:
        select = about(topics)
    i = 0
    while True:
        reference = choose(paragraphs, select, i)
        if not reference:
            print('No more paragraphs about', topics, 'are available.')
            return
        print('Type the following paragraph and then press enter/return.')
        print('If you only type part of it, you will be scored only on that part.\n')
        print(reference)
        print()

        start = datetime.now()
        typed = input()
        if not typed:
            print('Goodbye.')
            return
        print()

        elapsed = (datetime.now() - start).total_seconds()
        print("Nice work!")
        print('Words per minute:', wpm(typed, elapsed))
        print('Accuracy:        ', accuracy(typed, reference))

        print('\nPress enter/return for the next paragraph or type q to quit.')
        if input().strip() == 'q':
            return
        i += 1


@main
def run(*args):
    """Read in the command-line argument and calls corresponding functions."""
    import argparse
    parser = argparse.ArgumentParser(description="Typing Test")
    parser.add_argument('topic', help="Topic word", nargs='*')
    parser.add_argument('-t', help="Run typing test", action='store_true')

    args = parser.parse_args()
    if args.t:
        run_typing_test(args.topic)
