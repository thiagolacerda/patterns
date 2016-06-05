#!/usr/bin/env ruby

require 'json'

jsonMap = JSON.parse(File.read("#{__dir__}/thread_exec_config.json"), :symbolize_names => true)
execPath = "#{__dir__}/../build/release/patternsng"
execPathLeg = "#{__dir__}/../build/release/patterns"

numThreads = jsonMap[:threads]

def getLegacyDecoder(abbrev)
    "truckspaper" if abbrev == "t"
    "sortedfile" if abbrev == "c"
end

def execute(command, type, file, n, l, g, t)
    puts command
    output = `#{command}`
    puts output
    flocks = output.match(/Flocks found: (\d+)/)[1]
    execTime = output.match(/Duration: (\d+) s/)[1]
    file.puts "#{t};#{type};#{n};#{l};#{g};#{execTime};#{flocks}"
end

jsonMap[:confs].each do |execDescription|
    outFile = "#{execDescription[:name]}_thread_eval.txt"
    decoder = execDescription[:decoder]
    path = execDescription[:path]
    timeSlot = execDescription[:timeSlotSize]
    sep = execDescription[:sep]
    open(outFile, 'w') do |f|
        f.puts "Threads;Type;Number Flocks;Length;Grid;Time;Flocks"
        execDescription[:n].each do |n|
            execDescription[:l].each do |l|
                execDescription[:g].each do |g|
                    legacyBaseCommand = "#{execPathLeg} -n #{n} -d #{getLegacyDecoder(decoder)} -l #{l} - g #{g} -t #{timeSlot} -o"
                    if /trucks/ =~ execDescription[:name]
                        legacyBaseCommand += " -s 1 "
                    else
                        legacyBaseCommand += " -s 0 "
                    end

                    execute(legacyBaseCommand + path, "o", f, n, l, g, 1)
                    execute(legacyBaseCommand + " -b #{path}", "b", f, n, l, g, 1)
                    numThreads.each do |t|
                        threadsParam = ""
                        dataProc = "f"
                        if t != 1
                            threadsParam = "threads=#{t}"
                            dataProc = "pf"
                        end
                        command = "#{execPath} -c f -d #{decoder} -l g -p #{dataProc} --cp \"file=#{path} separator=\\#{sep}\" --lp \"timeSlotSize=#{timeSlot} patternLength=#{l}\" --pp \"gridSize=#{g} trajectoriesPerFlock=#{n} flockLength=#{l} #{threadsParam}\""
                        execute(command, "t", f, n, l, g, t)
                    end
                end
            end
        end
    end
end

