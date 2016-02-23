#!/usr/bin/env ruby
require 'benchmark'
require 'optparse'
require 'ostruct'

class ArgumentParser

    def self.parse(args)
        options = OpenStruct.new

        options.executablePaths = []
        options.numberOfTrajectories = []
        options.flockLengths = []
        options.gridSizes = []
        options.databasePath = ''
        options.arguments = []

        optParser = OptionParser.new do |opts|
            opts.banner = 'Usage: compare.rb [options]'

            opts.separator ''
            opts.separator 'Specific options:'

            # Mandatory argument.
            opts.on('-e <executable_paths>', Array, 'array of executable paths') do |paths|
               options.executablePaths = paths
            end

            opts.on('-n <number_of_trajectories>', Array, 'array of number of trajectories per execution') do |trajectories|
               options.numberOfTrajectories = trajectories
            end

            opts.on('-l <flock_lengths>', Array, 'array of flock lengths per execution') do |lengths|
               options.flockLengths = lengths
            end

            opts.on('-g <grid_sizes>', Array, 'array of grid sizes per execution') do |grids|
               options.gridSizes = grids
            end

            opts.on('-d <database_path>', String, 'database path') do |path|
               options.databasePath = path
            end

            opts.on('-o <output_files>', Array, 'output file') do |files|
                options.files = files
            end

            opts.on('-a <arguments>', String, 'extra arguments') do |arguments|
                options.arguments = arguments.split(',', -1).map { |arg| arg.split(';') }
            end

            opts.on_tail('-h', '--help', 'Show this message') do
                puts opts
                exit
            end
        end

        optParser.parse!(args)
        options
    end
end

def compare(options)
    options.executablePaths.zip(options.files, options.arguments).each do |path, outputFile, arguments|
        executable = File.basename(path)
        args = arguments.join(' ')
        open(outputFile, "w") do |f|
            f.puts "Number Flocks;Length;Grid;Time;Flocks"
            commandOut = File.open("#{outputFile}.commands", "w")
            options.numberOfTrajectories.each do |n|
                options.flockLengths.each do |l|
                    options.gridSizes.each do |g|
                        command = "#{path}"
                        if executable == "patterns"
                            command += " -n #{n} -l #{l} -g #{g} #{args} #{options.databasePath}"
                        else
                            command += " #{args}, #{n}, #{l}, #{g} #{options.databasePath}"
                        end
                        puts "command: #{command}"
                        startTime = Time.now
                        output = ""
                        executionTime = Benchmark.measure { output = `#{command}` }
                        puts output
                        commandOut.puts output
                        flocks = output.match(/Flocks found: (\d+)/)[1]
                        puts "#{executionTime}"
                        totalTime = executionTime.total.round #[:user_sec].to_f.round + executionTime[:sys_sec].to_f.round
                        diff = Time.now - startTime
                        puts "total time: #{totalTime}"
                        puts "diff startTime - endTime: #{diff}"
                        f.puts "#{n};#{l};#{g};#{totalTime};#{flocks}"
                        f.flush
                        commandOut.flush
                    end
                end
            end
            commandOut.close()
        end
    end
end

options = ArgumentParser.parse(ARGV)
puts 'missing executable paths array' if options.executablePaths.empty?
puts 'missing number of trajectories per flock array' if options.numberOfTrajectories.empty?
puts 'missing length of flocks array' if options.flockLengths.empty?
puts 'missing grid sizes array' if options.gridSizes.empty?
puts 'missing database path' if options.databasePath.empty?
if options.executablePaths.empty? || options.numberOfTrajectories.empty? || options.flockLengths.empty? ||
        options.gridSizes.empty? || options.databasePath.empty?
    exit
end

compare(options)
