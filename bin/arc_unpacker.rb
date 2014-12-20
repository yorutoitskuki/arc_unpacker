#!/usr/bin/ruby -W2
require 'optparse'
require 'fileutils'
require_relative '../lib/archive_factory'

# CLI frontend
class CLI
  def initialize
    parse_options
  end

  def run
    FileUtils.mkpath(@options[:output_path]) unless @options[:output_path].nil?
    begin
      run_internal
    rescue StandardError => e
      puts 'Error: ' + e.message.to_s
      exit(1)
    end
  end

  private

  def run_internal
    archive = ArchiveFactory.get(@options[:format])
    archive.read(@options[:input_path])
    archive.extract(@options[:output_path], @options[:verbose])
  end

  def parse_options
    @options =
    {
      input_path: nil,
      output_path: nil,
      verbose: true
    }

    opt_parser = OptionParser.new do |opts|
      opts.banner = format(
        'Usage: %s INPUT_PATH OUTPUT_DIR',
        File.basename(__FILE__))

      opts.on(
        '-f',
        '--fmt FORMAT',
        ArchiveFactory.format_strings,
        format(
          'Select decryption type (%s)',
          ArchiveFactory.format_strings.join(', '))) \
      do |format|
        @options[:format] = format
      end

      opts.separator ''
      opts.on('-q', '--quiet', 'Suppress output') { @options[:verbose] = false }
      opts.on_tail('-h', '--help', 'Show this message') { puts opts && exit }
    end

    begin
      opt_parser.parse!

      if @options[:format].nil?
        fail OptionParser::MissingArgument, 'Must specify decryption format.'
      end
    rescue StandardError => e
      raise e unless e.class.name.start_with?('OptionParser')
      puts e.message, '', opt_parser
      exit(1)
    end

    if ARGV.length != 2
      puts opt_parser
      exit(1)
    end

    @options[:input_path] = ARGV.first
    @options[:output_path] = ARGV.last
  end
end

CLI.new.run