# State record: CurrentHandStream

Original section of [current-task-state.json](<../../../../work/current-task-state.json>). Captured 2026-10-14T01:55:24.397473+02:00.

This is a verbatim structured record, not a new validation. Earlier fields named Current/Live can describe historical builds.

````json
{
  "DllSha256": "DBA742675D90BEFE8A56CA4876F56BF3BF07B0D67E6702B3376C9514A7FE2865",
  "Reports": [
    {
      "Passed": true,
      "Sha256": "5DF0F8C46328B0BB805C45383909A2658EC772409CA2FB8575A39B4E85964380",
      "Report": "arm-stream-observations.json"
    },
    {
      "Passed": true,
      "Sha256": "55112146A6FF27E4A8956920E44E9091935AE52D078DD6668129536A58D65D2B",
      "Report": "arm-stream-verification.json"
    },
    {
      "Passed": true,
      "Sha256": "259C91670CDC9B7A4D64F4504EEBE5FE1F48BCCAC0712A7A703541D7AA88D56A",
      "Report": "arm-stream-guards-verification.json"
    },
    {
      "Passed": true,
      "Sha256": "536E9809615EBE20A2E5DA831BD3A7950B0A002E342697DCC50CCEB4B790BE1C",
      "Report": "avatar-weapon-observations.json"
    },
    {
      "Passed": true,
      "Sha256": "F4794F02D20A6B0E204FFF684F2FDD112E8B49689AF1B5B71EBD31EC26D89B48",
      "Report": "avatar-weapon-verification.json"
    },
    {
      "Passed": true,
      "Sha256": "2C7B8B2F334B314A1F6CBE0CC8DA5AE8B80E598E13F210005205529399B852A7",
      "Report": "avatar-weapon-client-death-verification.json"
    },
    {
      "Passed": true,
      "Sha256": "AE86AF66762275FA2CA3EA3D1A96D08F4C40A89414A74E7A861587E29305B4F9",
      "Report": "shared-death-client-health-damage-equipment-verification.json"
    },
    {
      "Passed": true,
      "Sha256": "CA3EF890B144DE70B11D29A0D01E3B4E0DCDF9BD535E66F66ECB012B95FCC513",
      "Report": "enemy-melee-client-counterattack-recovery-arms-verification.json"
    },
    {
      "Passed": true,
      "Sha256": "50A9F6FD42C2012FD72546B84058DC719F2F35B9522AB842AD64F3F00E77B88A",
      "Report": "player-arm-observation-verification.json"
    },
    {
      "Passed": true,
      "Sha256": "5E05E0B42C11D3E08848794DD2B2EF3C39D52954D5905B39269E1E02BA120B2B",
      "Report": "baseline-integrity-verification.json"
    }
  ],
  "ProbeSha256": "11F01DC0C54F8C4A99DD235D8FC5EB5E957BB15CC91DEA4C18772FD17B4B9BB4",
  "Scope": "21 semantic right-arm/hand joints; 2604 independent native joint comparisons. Sword lifecycle, client natural death and combined ordinary melee/counterattack/shared retry pass. Visual Corvo finger grip remains too open; correct numerical transmission does not establish a correct closed sword grip.",
  "Screenshot": "research/screens/avatar-hand-grip-DBA7.png",
  "SourceBackup": "work/native-hand-stream-DBA7",
  "VisualGripPassed": false,
  "ObservedAt": "2026-10-09T07:55:12.8975154+02:00"
}
````
