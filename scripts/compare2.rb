#!/usr/bin/env ruby
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
    options.executablePaths.zip(options.files).each do |path, outputFile|
        executable = File.basename(path)
        open(outputFile, "w") do |f|
            options.numberOfTrajectories.each do |n|
                options.flockLengths.each do |l|
                    options.gridSizes.each do |g|
                        command = "#{path}"
                        if executable == "patterns"
                            command += " -c -n #{n} -l #{l} -g #{g} -s 1 -d truckspaper -o -b #{options.databasePath}"
                        else
                            command += " p, #{n}, #{l}, #{g} #{options.databasePath}"
                        end
                        puts "command: #{command}"
                        startTime = Time.now
                        output = `#{command}`
                        puts output
                        flocks = output.match(/Flocks found: (\d+)/)[1]
                        diff = Time.now - startTime
                        f.puts "#{n};#{l};#{g};#{diff};#{flocks}"
                        f.flush
                    end
                end
            end
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
