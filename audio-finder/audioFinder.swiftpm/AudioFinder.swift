import SwiftUI
import AVFoundation
import Speech

struct AudioFinder: View {
    
    @State var audioURL = Bundle.main.url(forResource: "chicanery", withExtension: "mp3")
    @State var timestamps : Dictionary<String,Array<Float>>? = ["" : [0]]
    @State var audioPlayer : AVAudioPlayer? = nil
    @State var searchedWord : String = ""
    @State var receivedTimes : Array<Float> = [0]
    @State var times : Array<Float> = [0]
    @State var readyToSearch : Bool = false
    @State var isSpinning: Bool = false
    
    // For audioplayer ->
    @State var progress : Float = 0
    @State var duration : Float = 0
    @State var timeIndex : Int = 0
    @State var isPlaying: Bool = false
    @State var activityIndicator : UIView = UIActivityIndicatorView(style: .medium)
    
    var body: some View {
        VStack {
            
            TextField( "Search for a word here!", text: $searchedWord )
                .padding(.horizontal, 100)
            
            VStack {
                HStack {
                    Slider(value: $progress, in: 0...duration)
                    Spacer()
                    Text( "00:" + String(format: "%02d", Int(floor(progress/100))) )
                } // HStack
                .padding(.horizontal, 50)
                
                Button(action: { toggleAudio() }) {
                    Label("", systemImage: isPlaying ? "pause.fill" : "play.fill")
                        .imageScale(.large)
                } // Button
                .padding(.top, 10)
            }
            .padding(.top, 30)
            
            Spacer()
            
            readyToSearch ?
            HStack {
                Button(action: { findWord() }) {
                    Text("Find Word")
                } // Button
                
                Spacer()
                
                Button(action: { findNext() }) {
                    Text("Find Next")
                } // Button
            }
            .padding(.horizontal, 70)
            : nil
            
            isSpinning ? ProgressView() : nil
            
            Spacer()
            
        } //VStack
        .padding(.top)
        
    } // View
    
    func recognizeFile(url: URL) {
        let audioRecognizer = SFSpeechRecognizer()
        let request = SFSpeechURLRecognitionRequest(url: url)
        
        audioRecognizer!.supportsOnDeviceRecognition = true
        audioRecognizer!.recognitionTask(with: request) { (result, error) in
            guard let result else {
                print("FAILED")
                return
            } // guard
            
            if result.isFinal {
                print("Result is Final")
                let transcription = result.bestTranscription.segments
                analyzeAudio(transcription: transcription)
            } // if
        } // Recognizer
    } // recognizeFile
    
    func analyzeAudio ( transcription : [SFTranscriptionSegment] ) {
        //recognizeFile(url: audioURL!)
        
        for segment in transcription {
            
            if (segment.substring.contains(" ")) {
                let phrase = segment.substring.split(separator: " ")
                
                for word in phrase {
                    addToTimestamps(substring: String(word).lowercased(), timestamp: floor(Float(segment.timestamp)*100))
                } // for
            } else {
                addToTimestamps(substring: String(segment.substring).lowercased(), timestamp: floor(Float(segment.timestamp)*100))
            } // else
        } // for
        
        isSpinning = false
        readyToSearch = true
        
    } // analyzeAudio
    
    func addToTimestamps (substring : String, timestamp : Float) {
        
        if ( timestamps![substring] == nil ) {
            timestamps![substring] = [timestamp]
            
        } else {
            timestamps![substring]!.append(timestamp)
            
        } // else
    } // addToTimestamps
    
    func findWord() {
        
        searchedWord = searchedWord.lowercased()
        
        if ( timestamps![searchedWord] == nil ) {
            //times = [progress]
            print("Word not found..")
        } else {
            times = timestamps![searchedWord]!
            findNext()
        } // else
    } // findWord
    
    func findNext () {
        
        if times != receivedTimes {
            receivedTimes = times
            timeIndex = 0
        } // if
            
        if timeIndex == times.count {
            timeIndex = 0
            audioPlayer!.currentTime = Float64(times[timeIndex]/100)
            progress = Float(audioPlayer!.currentTime)*100
            timeIndex += 1
            
        } else {
            audioPlayer!.currentTime = Float64(times[timeIndex]/100)
            progress = Float(audioPlayer!.currentTime)*100
            timeIndex += 1
            
        } // else
    } // findNext
    
    func toggleAudio() {
        if (audioPlayer == nil) {
            getAudio()
            isSpinning = true
            recognizeFile(url: audioURL!)
        } // if
        
        if ( isPlaying ) {
            audioPlayer!.stop()
            isPlaying = false
            
        } else {
            audioPlayer!.currentTime = Double(progress)
            audioPlayer!.play()
            isPlaying = true
            
        } // else
    } // toggleAudio
    
    func getAudio() {
        if let audioURL = Bundle.main.url(forResource: "chicanery", withExtension: "mp3") {
            do {
                try audioPlayer = AVAudioPlayer(contentsOf: audioURL)
                audioPlayer?.numberOfLoops = 0
                
            } catch {
                print("Couldn't load audio. Error \(error)")
                
            } // catch
        } // if
        
        duration = floor(Float(audioPlayer!.duration)*100)
        
        _ = Timer.scheduledTimer(withTimeInterval: 1.0, repeats: true) { timer in
            
            if ( (abs(progress - floor(Float(audioPlayer!.currentTime)*100)) > 110) &&  Float(audioPlayer!.currentTime) != 0 ) {
                jumpTo()
            } else {
                progress = floor(Float(audioPlayer!.currentTime)*100)
            } // else
            
        } // timer
    } // getAudio
    
    func jumpTo () {
        audioPlayer!.stop()
        audioPlayer!.currentTime = TimeInterval(progress/100)
        
        if (isPlaying) {
            audioPlayer!.play()
            
        } // if
    } // jumpTo
} // AudioFinder

struct AudioFinder_Preview: PreviewProvider {
    static var previews: some View {
        AudioFinder()
    }
}
